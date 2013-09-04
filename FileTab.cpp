#include "mainwindow.h"

///////////////////
//
//  FileTab
//
///////////////////

FileTab::FileTab(QWidget *parent) :
    QWidget(parent)
{
    grid = new QGridLayout(this);
    editor = new HighlightedEdit(this);

    grid->addWidget(editor);
    grid->setMargin(3);
    editor->setFont(QFont("monospace", 10));

    isTextChanged = false;
}

FileTab::~FileTab()
{
    delete editor;
    delete grid;
}

void FileTab::SetEditorFont(QFont font)
{
    editor->setFont(font);
}

void FileTab::ConnectSignals(QWidget *parent)
{
    connect(editor, SIGNAL(textChanged()), parent, SLOT(tabCurrentModified()));
}
