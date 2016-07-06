#include "CAlgorithmPanel.h"

CAlgorithmPanel::CAlgorithmPanel(CBufor *bufor, QWidget *parent) :
    QWidget(parent)
{
    imageContainer = &(bufor->container);
	pointContainer = &(bufor->point_container);
	radiusContainer = &(bufor->radius_container);
    //powiąż z buforem
    connect(bufor, SIGNAL(imageWasAdded()), this, SLOT(updateImageFiltersDatabase()));

    createRadioGroup();

    imageFilters = new QMultiMap<QString, QListWidgetItem>;

    infoLabel = new QLabel;

    createAlgorithmItems();

    buttonSave  = new QPushButton("Save");
    buttonSave->setMinimumHeight(80);
	connect(buttonSave, SIGNAL(clicked()), this, SLOT(saveAs()));

    processignImagePath = "NONE";

    createLayout();
}

bool CAlgorithmPanel::isSingleMode()
{
    if(typeProcessingRadioGroup->checkedButton() == singleRadio)
        return true;
    else
        return false;
}

void CAlgorithmPanel::showImageProcesingMode()
{
    //messageBox.information(0,"VSDF","sdasdsad");
    if(isSingleMode())
    {
        processignImagePath = "NONE";
        infoLabel->setText(QString("<br>Processing image path: <br> <i>%1</i> <br><br> ").arg(processignImagePath));
    }
    else
    {
        processignImagePath = "MULTI_MODE";
        infoLabel->setText("");
    }

    refreshUsedAlgorithmList();
}

void CAlgorithmPanel::updateImageFiltersDatabase()
{
    /*
    QString lastAdded;

    foreach (QString string, imageContainer->keys())
        lastAdded = string;
    */
}

void CAlgorithmPanel::createLayout()
{
    thisLayout = new QVBoxLayout;
    thisLayout->addWidget(singleRadio);
    thisLayout->addWidget(multiRadio);
    thisLayout->addWidget(infoLabel);
    thisLayout->addWidget(createCheckAlgorithmBox());
    thisLayout->addWidget(createButtonBox());
    thisLayout->addWidget(createUsedAlgorithmBox());
    thisLayout->addWidget(buttonSave);
    this->setLayout(thisLayout);
}

void CAlgorithmPanel::createRadioGroup()
{
    typeProcessingRadioGroup = new QButtonGroup;
    singleRadio = new QRadioButton("Single image processing");
    multiRadio = new QRadioButton("Multi image processing");
    typeProcessingRadioGroup->exclusive();
    typeProcessingRadioGroup->addButton(singleRadio);
    typeProcessingRadioGroup->addButton(multiRadio);

    connect(typeProcessingRadioGroup, SIGNAL(buttonClicked(int)), this, SLOT(showImageProcesingMode()));
}

void CAlgorithmPanel::createAlgorithmItems()
{
    greyScale  = new QListWidgetItem("Gray scale");
	findAndDrawCircles = new QListWidgetItem("Find and draw circles");
    negative = new QListWidgetItem("Negative");
}

QGroupBox *CAlgorithmPanel::createCheckAlgorithmBox()
{
     QGroupBox *checkAlgorithmBox = new QGroupBox(tr("Check algorithms:"));

     availableAlgorithms = new QListWidget;
     availableAlgorithms->addItem(greyScale);
	 availableAlgorithms->addItem(findAndDrawCircles);
     availableAlgorithms->addItem(negative);

     QVBoxLayout *vbox = new QVBoxLayout;

     vbox->addWidget(availableAlgorithms);
     vbox->addStretch(1);

     checkAlgorithmBox->setLayout(vbox);

     return checkAlgorithmBox;
}

QGroupBox *CAlgorithmPanel::createButtonBox()
{
    QGroupBox *buttonBox = new QGroupBox;

    buttonApply  = new QPushButton("Apply");
    connect(buttonApply, SIGNAL(clicked()), this, SLOT(activateAlgorithm()));

    buttonRemove = new QPushButton("Remove");
    connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeAlgorithm()));

    QHBoxLayout *hbox = new QHBoxLayout;

    hbox->addWidget(buttonApply);
    //hbox->addWidget(buttonDown);

    buttonBox->setLayout(hbox);

    return buttonBox;
}

QGroupBox *CAlgorithmPanel::createUsedAlgorithmBox()
{
     QGroupBox *usedAlgorithmBox = new QGroupBox(tr("Used algorithms:"));

     usedAlgorithms = new QListWidget;

     QVBoxLayout *vbox = new QVBoxLayout;

     vbox->addWidget(usedAlgorithms);
      vbox->addStretch(1);

     usedAlgorithmBox->setLayout(vbox);

     return usedAlgorithmBox;
}

void CAlgorithmPanel::activateAlgorithm()
{
    if (processignImagePath == "NONE")
    {
        if (typeProcessingRadioGroup->checkedButton() == 0)
            messageBox.information(0,"VSDF","Select processing mode (single or multi).");
        else
            messageBox.information(0,"VSDF","Click on image in MDI adrea you want to process.");
    }
    else if(availableAlgorithms->selectedItems().size() == 0)
    {
        messageBox.information(0,"VSDF","Select algorithm from list.");
    }
    else if(processignImagePath == "MULTI_MODE")
    {
        QString algorithm = availableAlgorithms->currentItem()->text();

        QHash<QString, QImage> tempHash;
		QHash<QString, QPoint> tempHashPoint;
		QHash<QString, int> tempHashRadius;
		QPoint tempPoint;
		int radius;

        QHash<QString, QImage>::const_iterator iterator = imageContainer->constBegin();
        while (iterator != imageContainer->constEnd())
        {
            QImage imageWithAlgorithm;
            imageWithAlgorithm = iterator.value();

            if(algorithm == "Gray scale")
                imageWithAlgorithm = COpenCV::toGrayScale(imageWithAlgorithm);
            else if(algorithm == "Negative")
                imageWithAlgorithm = COpenCV::toNegative(imageWithAlgorithm);
			else if(algorithm == "Find and draw circles")
				imageWithAlgorithm = COpenCV::findAndDrawCircles(imageWithAlgorithm, &tempPoint, &radius);
            else{}

            QString nameOfImageWithAlgorithm = iterator.key();
            nameOfImageWithAlgorithm.append("_") += algorithm;

            QString nameOfImageWithAlgorithmChecked = nameOfImageWithAlgorithm;

            int i = 1;
            while(imageContainer->contains(nameOfImageWithAlgorithmChecked))
            {
                  nameOfImageWithAlgorithmChecked = nameOfImageWithAlgorithm + (QString::number(i));
                  i++;
            }

            tempHash.insert(nameOfImageWithAlgorithmChecked, imageWithAlgorithm);
			if(algorithm == "Find and draw circles")
			{
				tempHashPoint.insert(nameOfImageWithAlgorithmChecked, tempPoint);
				tempHashRadius.insert(nameOfImageWithAlgorithmChecked, radius);
			}
            iterator++;
        }

        imageContainer->unite(tempHash);
		pointContainer->unite(tempHashPoint);
		radiusContainer->unite(tempHashRadius);

        QList<QString> newImages = tempHash.uniqueKeys();

        for (int i=0; i < newImages.size(); ++i)
        {
            emit alorithmWasActivated("", newImages.at(i));
        }

        QListWidgetItem newItem(algorithm);
        imageFilters->insert(processignImagePath, newItem);

        refreshUsedAlgorithmList();
    }
    else
    {
		QPoint tempPoint;
		int radius;
        QString algorithm = availableAlgorithms->currentItem()->text();

        QImage imageWithAlgorithm;
        imageWithAlgorithm = imageContainer->value(processignImagePath);

        if(algorithm == "Gray scale")
            imageWithAlgorithm = COpenCV::toGrayScale(imageWithAlgorithm);
        else if(algorithm == "Negative")
            imageWithAlgorithm = COpenCV::toNegative(imageWithAlgorithm);
		else if(algorithm == "Find and draw circles")
			imageWithAlgorithm = COpenCV::findAndDrawCircles(imageWithAlgorithm, &tempPoint, &radius);
        else{}

        QString nameOfImageWithAlgorithm = processignImagePath;
        nameOfImageWithAlgorithm.append("_") += algorithm;

        QString nameOfImageWithAlgorithmChecked = nameOfImageWithAlgorithm;

        int i = 1;
        while(imageContainer->contains(nameOfImageWithAlgorithmChecked))
        {
              nameOfImageWithAlgorithmChecked = nameOfImageWithAlgorithm + (QString::number(i));
              i++;
        }

        QString imageInActiveWindow = processignImagePath;
        QString addText = imageFilters->value(processignImagePath).text();

        if (!addText.isEmpty())
            imageInActiveWindow.append("_") += addText;

        imageContainer->insert(nameOfImageWithAlgorithmChecked, imageWithAlgorithm);
		if(algorithm == "Find and draw circles")
		{
			pointContainer->insert(nameOfImageWithAlgorithmChecked, tempPoint);
			radiusContainer->insert(nameOfImageWithAlgorithmChecked, radius);
		}

        emit alorithmWasActivated(imageInActiveWindow, nameOfImageWithAlgorithmChecked);

        QListWidgetItem newItem(algorithm);
        imageFilters->insert(processignImagePath, newItem);

        refreshUsedAlgorithmList();
    }
}

void CAlgorithmPanel::removeAlgorithm()
{
    if(usedAlgorithms->selectedItems().size() != 0)
    {
        usedAlgorithms->takeItem(usedAlgorithms->row(usedAlgorithms->currentItem()));
        imageFilters->remove(processignImagePath);

        for(int i = 0; i < usedAlgorithms->count(); i++)
        {
            QListWidgetItem newItem(usedAlgorithms->item(i)->text());
            imageFilters->insert(processignImagePath, newItem);
        }
    }
    else
    {
        messageBox.information(0,"VSDF","First select used algorithm to remove.");
    }
}

void CAlgorithmPanel::setActiveFilePath(QString path, QString removedPath)
{

    if (!removedPath.isNull())
        imageFilters->remove(removedPath);

    if ((removedPath.isNull()) || (processignImagePath == removedPath))
    {
        if(isSingleMode())
        {
            processignImagePath = path;
            infoLabel->setText(QString("<br>Processing image path: <br> <i>%1</i> <br><br> ").arg(path));
            refreshUsedAlgorithmList();
        }
    }
}

void CAlgorithmPanel::refreshUsedAlgorithmList()
{
    usedAlgorithms->clear();

    QList<QListWidgetItem> list = imageFilters->values(processignImagePath);

    for (int i=0; i < list.size(); ++i)
        usedAlgorithms->addItem(list.at(list.size()-1-i).text());
}

void CAlgorithmPanel::saveAs()
{
	emit save();
}