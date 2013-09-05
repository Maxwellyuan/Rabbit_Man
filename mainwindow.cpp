#include "mainwindow.h"
#include "ui_mainwindow.h"

/////////////////
//
//  MainWindow
//
/////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TabIcon = new QIcon(":/new/prefix1/document.gif");

    ConnectSignals();
    ConnectToolbar();

    ui->statusBar->showMessage(tr("Welcome to Rabbit Man !"), 5000);
}

MainWindow::~MainWindow()
{
    delete TabIcon;
    delete ui;
}

void MainWindow::ConnectSignals(void)
{
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClose(int)));
}

void MainWindow::ConnectToolbar(void)
{
    ui->toolBar->addAction(ui->actionNew);
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addAction(ui->actionSaveAs);
    ui->toolBar->addAction(ui->actionSave);
    ui->toolBar->addSeparator();
}

void MainWindow::tabClose(int index)
{
    ui->statusBar->showMessage(tr("The tab having opened \"%1\" is closed.")
                               .arg(ui->tabWidget->tabText(index)), 5000);
    delete ui->tabWidget->widget(index);
}

//新建文件和标签
FileTab *MainWindow::tabNew(const QString tabtext)
{
    ui->tabWidget->addTab(new FileTab(this), tabtext);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
    ui->tabWidget->setTabIcon(ui->tabWidget->currentIndex(), *TabIcon);
    return ((FileTab *)ui->tabWidget->currentWidget());
}

void MainWindow::tabCurrentModified(void)
{
    int index = ui->tabWidget->currentIndex();
    QString text(ui->tabWidget->tabText(index));
    FileTab *tab = (FileTab *)ui->tabWidget->widget(index);

    qDebug() << tab->GetFlagIsChanged();

    if (!tab->GetFlagIsChanged()) {
        text.append(tr(" *"));
        ui->tabWidget->setTabText(index, text);
        tab->SetChangedFlag(true);
    }
}

void MainWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

//新建一个文件
void MainWindow::on_actionNew_triggered()
{
    tabNew("untitled");
    ui->statusBar->showMessage(tr("File"), 5000);
}

//打开文件
void MainWindow::on_actionOpen_triggered()
{
    QFile *file;
    FileTab *tab;
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                "", tr("C/C++ Source(*.c *.cpp);;Text files (*.txt)"));

    if (!path.isEmpty()) {
        file = new QFile(path);
        if (file->open(QFile::ReadOnly | QFile::Text)) {
            tab = (FileTab *)ui->tabWidget->currentWidget();
            if (!tab || !tab->GetDataFromEdit().isEmpty()) {
                tab = tabNew(0);
            }
            tab->SendDataToEdit(file->readAll());

            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),
                                      QFileInfo(file->fileName()).fileName());
            ui->statusBar->showMessage(path, 5000);
            tab->ConnectSignals(this);
        }
    }
}

void MainWindow::on_actionFontEditor_triggered()
{
    int i;
    QFont font;
    FileTab *tab;

    font = QFontDialog::getFont(0, QFont("monospace", 10));
    for (i = 0;i < ui->tabWidget->count();i++) {
        tab = (FileTab *)ui->tabWidget->widget(i);
        tab->SetEditorFont(font);
    }
}

void MainWindow::on_actionZoomIn_triggered()
{
    ((FileTab *)ui->tabWidget->currentWidget())->Zooming(true);
}

void MainWindow::on_actionZoomOut_triggered()
{
    ((FileTab *)ui->tabWidget->currentWidget())->Zooming(false);
}
