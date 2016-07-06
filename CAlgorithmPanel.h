#ifndef CALGORITHMPANEL_H
#define CALGORITHMPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QListWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>

#include "COpenCV.h"
#include "CBufor.h"

class CAlgorithmPanel : public QWidget
{
    Q_OBJECT

public:

    explicit CAlgorithmPanel(CBufor *bufor = 0, QWidget *parent = 0);

    QMultiMap<QString, QListWidgetItem>  *imageFilters;

private:

    QHash<QString, QImage>               *imageContainer; //wskazuje na bufor w pamięci
	QHash<QString, QPoint>				 *pointContainer;
	QHash<QString, int>				     *radiusContainer;
    QList<QListWidgetItem>               *values;

    QButtonGroup    *typeProcessingRadioGroup;

    QRadioButton    *singleRadio;
    QRadioButton    *multiRadio;

    QVBoxLayout     *thisLayout;

    QListWidget     *usedAlgorithms;
    QListWidget     *availableAlgorithms;

    QListWidgetItem *greyScale;
	QListWidgetItem *findAndDrawCircles;
    QListWidgetItem *negative;
    QListWidgetItem *newItem;

    QGroupBox       *buttonBox;

    QPushButton     *buttonApply;
    QPushButton     *buttonRemove;
    QPushButton     *buttonSave;

    QMessageBox      messageBox;


    QGroupBox       *createCheckAlgorithmBox();
    QGroupBox       *createUsedAlgorithmBox();
    QGroupBox       *createButtonBox();
    QLineEdit       *processingImagePathView;
    QString          processignImagePath;
    QLabel          *infoLabel;

    void             createAlgorithmItems();
    void             refreshUsedAlgorithmList();
    void             createRadioGroup();
    void             createLayout();
    bool             isSingleMode();

signals:
	void		save();
    void        alorithmWasActivated(QString, QString);

public slots:
    void        setActiveFilePath(QString path, QString removedPath = 0);

private slots:
	void		saveAs();
    void        activateAlgorithm();
    void        removeAlgorithm();
    void        updateImageFiltersDatabase();
    void        showImageProcesingMode();
};

#endif // CALGORITHMPANEL_H
