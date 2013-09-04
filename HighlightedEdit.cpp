#include "mainwindow.h"

///////////////////////
//
//  HighlightedEdit
//
///////////////////////

HighlightedEdit::HighlightedEdit(QWidget *parent) :
    QPlainTextEdit(parent)
{
    LineNumberArea = new LineNumber(this);

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(HighlightingCurrentLine()));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(const QRect &, int)), this, SLOT(UpdateLineNumberArea(const QRect &, int)));

    UpdateLineNumberAreaWidth(0);
    HighlightingCurrentLine();
}

void HighlightedEdit::LineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(LineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, LineNumberArea->width(), fontMetrics().height(),
                          Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int HighlightedEdit::LineNumberAreaWidth(void)
{
    int digits = 1;
    int max = qMax(1, blockCount());

    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void HighlightedEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    LineNumberArea->setGeometry(QRect(cr.left(), cr.top(), LineNumberAreaWidth(), cr.height()));
}

void HighlightedEdit::UpdateLineNumberArea(const QRect &r, int dy)
{
    if (dy) {
        LineNumberArea->scroll(0, dy);
    } else {
        LineNumberArea->update(0, r.y(), LineNumberArea->width(), r.height());
    }

    if (r.contains(viewport()->rect())) {
        UpdateLineNumberAreaWidth(0);
    }
}

void HighlightedEdit::UpdateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(LineNumberAreaWidth(), 0, 0, 0);
}

void HighlightedEdit::HighlightingCurrentLine(void)
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}
