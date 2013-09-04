#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFontDialog>
#include <QPainter>
#include <QTextBlock>
#include <QSize>
#include <QDebug>

QT_BEGIN_NAMESPACE
class FileTab;
class HighlightedEdit;
class LineNumber;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

///////////////////
//
//  FileTab
//
///////////////////

class FileTab : public QWidget
{
    Q_OBJECT

public:
    FileTab(QWidget *parent = 0);
    ~FileTab();

    void SetEditorFont(QFont font);
    void ConnectSignals(QWidget *parent);

    void SendDataToEdit(const QString dat)
    { ((QTextEdit *)editor)->setPlainText(dat); }
    QString GetDataFromEdit(void)
    { return ((QTextEdit *)editor)->toPlainText(); }
    bool GetFlagIsChanged(void)
    { return isTextChanged; }
    void SetChangedFlag(bool f)
    { isTextChanged = f; }
    void Zooming(bool w) {
        if (w) ((QTextEdit *)editor)->zoomIn();
        else ((QTextEdit *)editor)->zoomOut();
    }

private:
    HighlightedEdit *editor;
    QGridLayout *grid;

    bool isTextChanged;
};

///////////////////////
//
//  HighlightedEdit
//
///////////////////////

class HighlightedEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    HighlightedEdit(QWidget *parent = 0);

    void LineNumberAreaPaintEvent(QPaintEvent *event);
    int LineNumberAreaWidth(void);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void UpdateLineNumberAreaWidth(int);
    void UpdateLineNumberArea(const QRect &r, int dy);
    void HighlightingCurrentLine(void);

private:
    QWidget *LineNumberArea;
};

/////////////////
//
//  LineNumber
//
/////////////////
class LineNumber : public QWidget
{
public:
    LineNumber(HighlightedEdit *editor0) : QWidget(editor0) {
        editor = editor0;
    }
    QSize sizeHint(void) const {
        return QSize(editor->LineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        editor->LineNumberAreaPaintEvent(event);
    }

private:
    HighlightedEdit *editor;
};

/////////////////
//
//  MainWindow
//
/////////////////

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ConnectSignals(void);
    void ConnectToolbar(void);

public slots:
    void tabCurrentModified(void);

private slots:
    void tabClose(int index);
    FileTab *tabNew(const QString tabtext);

    void on_actionAboutQt_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionFontEditor_triggered();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();

private:
    Ui::MainWindow *ui;
    QIcon *TabIcon;
};

#endif // MAINWINDOW_H
