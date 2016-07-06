#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mdichild.h"


#include <QtWidgets>
#include <QString>
#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	myBufor = new CBufor;

	mdiArea = new QMdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
	mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, true);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createDockWindow();
	createAlgorithmPanel();
	updateMenus();

	setWindowTitle(tr("VSDF"));
	setUnifiedTitleAndToolBarOnMac(true);
	resize(1400,800);
}


void MainWindow::open()
{
	filesList = QFileDialog::getOpenFileNames(this,tr("Select image"), QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), tr("Images (*.png *.jpg *.bmp)"));
	filesListIcon = filesList;

	numberOfImages = filesList.size();

	if (filesList.size() != 0)
	{
		info(QString("Please wait, loading images to buffer... 1 from %1").arg(numberOfImages));

		currentFile = filesList.takeLast();
		connect(&futureWatcher, SIGNAL(finished()), this, SLOT(onOpenImageFinished()));
		futureWatcher.setFuture(QtConcurrent::run(myBufor, &CBufor::addImage, currentFile));
	}

	/*
	QStringList::Iterator it = list.begin();

	while(it != list.end())
	{
	myBufor->addImage(*it);
	QIcon myIcon((QPixmap::fromImage(myBufor->getImageByPath(*it))));
	QListWidgetItem *newItem = new QListWidgetItem();
	newItem->setIcon(myIcon);
	newItem->setText(*it);
	newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	listImage->addItem(newItem);
	++it;
	}
	*/
}

void MainWindow::onOpenImageFinished()
{
	if (filesList.size() != 0)
	{
		currentFile = filesList.takeLast();

		int processedImg = numberOfImages - filesList.size();
		info(QString("Please wait, loading images to buffer... %1 from %2").arg(processedImg).arg(numberOfImages));

		if(!currentFile.isEmpty())
			futureWatcher.setFuture(QtConcurrent::run(myBufor, &CBufor::addImage, currentFile));
	}
	else
	{
		createIconsFromNewlyAddedImages();
	}
}

void MainWindow::createIconsFromNewlyAddedImages()
{
	while(filesListIcon.size() != 0)
	{
		info(QString("%1 images was loaded to buffer. Creating icon list...").arg(numberOfImages));

		QString last = filesListIcon.takeLast();
		addIconToBufferView(last);

		info(QString("Images were loaded successfully."));
	}
}

void MainWindow::addIconToBufferView(QString nameOfImageInBuffer)
{
	QIcon myIcon((QPixmap::fromImage(myBufor->getImageByPath(nameOfImageInBuffer))));
	QListWidgetItem *newItem = new QListWidgetItem();
	newItem->setIcon(myIcon);
	newItem->setText(nameOfImageInBuffer);
	newItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	listImage->addItem(newItem);
}

void MainWindow::updateMenus()
{
	bool hasMdiChild = (activeMdiChild() != 0);

	zoomInAct->setEnabled(hasMdiChild);
	zoomOutAct->setEnabled(hasMdiChild);
	zoomResetAct->setEnabled(hasMdiChild);
	openAllAct->setEnabled(hasItem());
	closeAct->setEnabled(hasMdiChild);
	closeAllAct->setEnabled(hasMdiChild);
	tileAct->setEnabled(hasMdiChild);
	cascadeAct->setEnabled(hasMdiChild);
	nextAct->setEnabled(hasMdiChild);
	previousAct->setEnabled(hasMdiChild);
	separatorAct->setVisible(hasMdiChild);
	fitToWindowAct->setEnabled(hasMdiChild);

	if (activeMdiChild())
		activeMdiChild()->windowMenuAction()->setChecked(true);
}



void MainWindow::createActions()
{
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setIcon(QIcon(ICON(open.png)));
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(tr("&Save..."), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setIcon(QIcon(ICON(save.png)));
	saveAct->setStatusTip(tr("Save an existing file"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setIcon(QIcon(ICON(exit.png)));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	removeAllWidget = new QAction(tr("Clear"), this);
	removeAllWidget->setShortcuts(QKeySequence::Quit);
	removeAllWidget->setStatusTip(tr("Remove all widget from the buffer store"));
	connect(removeAllWidget, SIGNAL(triggered()), this, SLOT(clearBufor()));

	zoomInAct = new QAction(tr("Zoom In"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setIcon(QIcon(ICON(ZoomIn.png)));
	zoomInAct->setStatusTip(tr("Zoom in an image"));

	zoomOutAct = new QAction(tr("Zoom Out"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setIcon(QIcon(ICON(zoomout.png)));
	zoomOutAct->setStatusTip(tr("Zoom out an image"));

	zoomResetAct = new QAction(tr("Normal Size"), this);
	zoomResetAct->setShortcut(tr("Ctrl+="));

	fitToWindowAct = new QAction(tr("Fit to Window"), this);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));

	openAllAct = new QAction(tr("Open the selected windows"), this);
	openAllAct->setCheckable(true);
	connect(openAllAct, SIGNAL(triggered()), this, SLOT(showItem()));

	closeAct = new QAction(tr("Cl&ose"), this);
	closeAct->setStatusTip(tr("Close the active window"));
	connect(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

	closeAllAct = new QAction(tr("Close &All"), this);
	closeAllAct->setStatusTip(tr("Close all the windows"));
	connect(closeAllAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

	tileAct = new QAction(tr("&Tile"), this);
	tileAct->setStatusTip(tr("Tile the windows"));
	connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

	cascadeAct = new QAction(tr("&Cascade"), this);
	cascadeAct->setStatusTip(tr("Cascade the windows"));
	connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

	nextAct = new QAction(tr("Ne&xt"), this);
	nextAct->setShortcuts(QKeySequence::NextChild);
	nextAct->setStatusTip(tr("Move the focus to the next window"));
	connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

	previousAct = new QAction(tr("Pre&vious"), this);
	previousAct->setShortcuts(QKeySequence::PreviousChild);
	previousAct->setStatusTip(tr("Move the focus to the previous "
		"window"));
	connect(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

	windowActionGroup = new QActionGroup(this); //tylko jedna pozycja w menu jest sprawdzana

	separatorAct = new QAction(this);
	separatorAct->setSeparator(true);
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addSeparator();
	fileMenu->addAction(removeAllWidget);
	fileMenu->addAction(exitAct);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addAction(zoomResetAct);
	viewMenu->addAction(fitToWindowAct);

	windowMenu = new QMenu(tr("&Window"));
	menuBar()->addMenu(windowMenu);

	windowMenu->addAction(openAllAct);
	windowMenu->addAction(closeAct);
	windowMenu->addAction((closeAllAct));
	windowMenu->addSeparator();
	windowMenu->addAction(tileAct);
	windowMenu->addAction(cascadeAct);
	windowMenu->addAction(previousAct);
	windowMenu->addAction(nextAct);

	menuBar()->addSeparator();
}

void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	viewToolBar = addToolBar(tr("View"));
	viewToolBar->addAction(zoomInAct);
	viewToolBar->addAction(zoomOutAct);
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createAlgorithmPanel()
{
	QDockWidget *algorithmPanelDock = new QDockWidget(tr(" Algorithm Panel "), this);
	algorithmPanelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	algorithmPanel = new CAlgorithmPanel(myBufor);

	connect(algorithmPanel, SIGNAL(alorithmWasActivated(QString, QString)), this, SLOT(updateMdiSubWindow(QString, QString)));
	connect(algorithmPanel, SIGNAL(save()), this, SLOT(save()));

	algorithmPanelDock->setWidget(algorithmPanel);
	addDockWidget(Qt::RightDockWidgetArea, algorithmPanelDock);
}

void MainWindow::createDockWindow()
{
	QDockWidget *dock = new QDockWidget(tr(" Buffered images: "), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	listImage = new QListWidget(dock);
	listImage->setIconSize(QSize(250,500));
	listImage->setContextMenuPolicy(Qt::CustomContextMenu);
	listImage->setSelectionMode(QAbstractItemView::MultiSelection);

	connect(listImage, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(showContextMenu()));
	connect(listImage, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(showItem()));
	connect(listImage, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(HandleItem()));

	dock->setWidget(listImage);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::showItem()
{
	foreach(QListWidgetItem *item, listImage->selectedItems())
	{
		QString fileName = item->text();

		if (!fileName.isEmpty())
		{
			QMdiSubWindow *existing = findMdiChild(fileName);
			if (existing)
			{
				mdiArea->setActiveSubWindow(existing);
				continue;
			}

			MdiChild *child = new MdiChild(myBufor, algorithmPanel);

			child->setImage(fileName);

			QScrollArea *scrollArea = new QScrollArea;

			//Zmiana domyślnego tła widgetu za pomocą klasy QPalette na ciemno szary
			QPalette pal = scrollArea->palette();
			pal.setBrush(QPalette::Background, QBrush(QColor(50, 50, 50)));
			scrollArea->setPalette(pal);

			//pozycjonowanie widgeta dziecka
			scrollArea->setAlignment(Qt::AlignCenter);
			scrollArea->setWidget(child);

			QMdiSubWindow *subWindow = new QMdiSubWindow;
			subWindow = mdiArea->addSubWindow(scrollArea);
			subWindow->setWindowTitle(fileName.section('/', -1));
			windowMenu->addAction(child->windowMenuAction());
			windowActionGroup->addAction(child->windowMenuAction());
			subWindow->show();

			statusBar()->showMessage(tr("File was loaded"), 2000);

			if(activeMdiChild())
			{
				connect(zoomInAct, SIGNAL(triggered()), child, SLOT(zoomIn()));
				connect(zoomOutAct, SIGNAL(triggered()), child, SLOT(zoomOut()));
				connect(zoomResetAct, SIGNAL(triggered()), child, SLOT(normalSize()));
				connect(child , SIGNAL(info(QString)), this , SLOT(info(QString)));
				connect(child , SIGNAL(setActiveFilePath(QString)), algorithmPanel , SLOT(setActiveFilePath(QString)));
			}
		}
	}
}

void MainWindow::showContextMenu()
{
	QMenu *contextMenu = new QMenu;
	QAction *deleteItem = new QAction(tr("Remove"), this);
	contextMenu->addAction(deleteItem);
	connect(deleteItem, SIGNAL(triggered()), this, SLOT(removeItem()));

	contextMenu->exec(QCursor::pos());
}

void MainWindow::removeItem()
{
	QString fileName = listImage->currentItem()->text();
	listImage->takeItem(listImage->row(listImage->currentItem()));

	QMdiSubWindow *existing = findMdiChild(fileName);
	mdiArea->setActiveSubWindow(existing);
	mdiArea->closeActiveSubWindow();

	myBufor->removeImageByPath(fileName);

	algorithmPanel->setActiveFilePath("NONE", fileName);
}

void MainWindow::clearBufor()
{
	listImage->clear();
	mdiArea->closeAllSubWindows();
	myBufor->removeAll();
	algorithmPanel->imageFilters->clear();
	algorithmPanel->setActiveFilePath("NONE");
}

MdiChild *MainWindow::activeMdiChild()
{
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
	{
		QScrollArea *scroll = qobject_cast<QScrollArea *>(activeSubWindow->widget());
		return qobject_cast<MdiChild *>(scroll->widget());
	}
	return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
	//QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
	QString canonicalFilePath = fileName;
	QMessageBox messageBox;


	foreach (QMdiSubWindow *window, mdiArea->subWindowList())
	{
		QScrollArea *scroll = qobject_cast<QScrollArea *>(window->widget());
		MdiChild *mdiChild = qobject_cast<MdiChild *>(scroll->widget());

		if (mdiChild->currentFile() == canonicalFilePath)
			return window;
	}
	return 0;
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::info(QString str)
{
	statusBar()->showMessage(str);
}

void MainWindow::updateMdiSubWindow(QString fileName, QString processedImage)
{
	QMdiSubWindow *subWindow = findMdiChild(fileName);

	if (subWindow)
	{
		QScrollArea *scroll = qobject_cast<QScrollArea *>(subWindow->widget());
		MdiChild *mdiChild = qobject_cast<MdiChild *>(scroll->widget());
		mdiChild->setImage(processedImage);
		subWindow->setWindowTitle(processedImage.section('/', -1));
	}
	addIconToBufferView(processedImage);
}

void MainWindow::save()
{
	if (listImage->count() == 0)
	{
		QMessageBox::warning( this, "Save failed", "Load new file" );
		return;
	}
	if(!listImage->currentItem())
	{
		QMessageBox::warning( this, "Save failed", "Choose saving file" );
		return;
	}

	autoButton = new QPushButton("Auto");
	manualButton = new QPushButton("Manual");
	
	buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(autoButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(manualButton, QDialogButtonBox::ActionRole);
	connect(autoButton, SIGNAL(clicked()), this, SLOT(autoSave()));
	connect(manualButton, SIGNAL(clicked()), this, SLOT(manualSave()));
	buttonBox->setFixedSize(400,100);
	autoButton->setFixedSize(200,100);
	manualButton->setFixedSize(200,100);
	buttonBox->setWindowTitle("Wybierz opcje zapisu");
	buttonBox->show();
}

void MainWindow::manualSave()
{
	QString savename;
	foreach(QListWidgetItem* item, listImage->selectedItems())
	{
		savename = item->text();

		QString savefileName = QFileDialog::getSaveFileName(this, tr("Save As"),
			(""), tr("PNG (*.png);;JPEG (*.JPEG);;Windows Bitmap (*.bmp)")); 
		if ( !savefileName.isEmpty() )
		{
			if ( !myBufor->getImageByPath(savename).save(savefileName, "png") )
				QMessageBox::warning( this, "Save failed", "Error saving file" );
			else
				info(QString("Zapisano obraz: ") + savefileName);
		}
	}
	buttonBox->close();
}

void MainWindow::autoSave()
{	
	QString savefileName = QFileDialog::getSaveFileName(this, tr("Save As"),
		(""), tr("PNG (*.png);;JPEG (*.JPEG);;Windows Bitmap (*.bmp)")); 

	QString suffix = savefileName.section(".", -1);
	QString temp = savefileName.section(".", 0, -2);

	if ( savefileName.isEmpty() )
	{
		QMessageBox::warning( this, "Save failed", "Error saving file" );
		buttonBox->activateWindow();
		return;
	}
	int k = 1;
	QList<QListWidgetItem *> lista = listImage->selectedItems();
	QList<QListWidgetItem *>::const_iterator i;
	for (i = lista.constBegin(); i != lista.constEnd(); ++i)
	{
		myBufor->getImageByPath((*i)->text()).save(savefileName, "png");
		info(QString("Zapisano obraz: ") + savefileName);
		savefileName = temp + QString("%1.").arg(k) + suffix;
		k++;
	}
	info(QString("Zakończono wczytywanie obrazów"));
	buttonBox->close();
}

void MainWindow::HandleItem()
{
	listImage->currentItem()->setSelected(!listImage->currentItem()->isSelected());
	listImage->setCurrentRow(listImage->currentRow(), QItemSelectionModel::Toggle);
	openAllAct->setEnabled(hasItem());
}

bool MainWindow::hasItem()
{
	QList<QListWidgetItem *> lista = listImage->selectedItems();
	if(lista.isEmpty())
		return false;
	return true;
}

MainWindow::~MainWindow()
{
	delete ui;
}
