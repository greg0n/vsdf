#include "mdichild.h"
#include <QtWidgets>
#include <QDebug>
#include <QString>

MdiChild::MdiChild(CBufor *bufor, CAlgorithmPanel *algorithmPanel, QWidget *parent) : QWidget(parent)
{
    dict = &(bufor->container);
	point = &(bufor->point_container);
	radius = &(bufor->radius_container);
    algoPanel = algorithmPanel;

    action = new QAction(this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered()), this, SLOT(show()));
    connect(action, SIGNAL(triggered()), this, SLOT(setFocus()));
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose); //przy zamknięciu podokna, niszczymy obiekt i zwalniamy pamięć
}

void MdiChild::setImage(const QString &name)
{
    action->setText(name.section('/', -1));
    curFile = name;

    image = dict->value(name);

    zoomReset();
}

//kopiuje obraz przekazany w parametrze
void MdiChild::setImage(const QImage & im)
{
    image = im; //kopiuje
    zoomReset(); //resetuje powiększenie
}

//rysuje obraz na powierzchni widgeta, wywoływana przy update()
void MdiChild::paintEvent(QPaintEvent *e)
{
    if(!image.isNull())
    {
        QPainter *paint = new QPainter(this);
        paint->scale(zoom , zoom); //skaluje układ współrzędnych
        paint->drawImage(0,0, image); //skaluje rysowany obraz
    }

}

void MdiChild::mouseMoveEvent(QMouseEvent *e)
{
    if(!image.isNull())
    {
        QPoint pos = e->pos()/zoom; //odczytanie aktualnego położenia kursora w widget
        //jeśli współrzędne zawierają się w obrazie
        if(pos.x()>0 && pos.x()<image.width() && pos.y()>0 && pos.y()<image.height())
        {
            QRgb color = image.pixel(pos);
            QString str = QString(tr("Point at [%1,%2]," "RGB Color [%3, %4, %5]")).arg(pos.x()).arg(pos.y()).arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color));
            emit info(str); //wyemitowanie sygnału
        }
    }
}

void MdiChild::mouseReleaseEvent(QMouseEvent *)
{
    if(!image.isNull())
    {
        QString str = QString(tr("Image [%1], ""Size [%2 x %3]")).arg(curFile).arg(image.width()).arg(image.height());
        emit info(str);
        emit setActiveFilePath(curFile);
		if(curFile.section('_', -1) == "Find and draw circles")
		{
			QPoint tempPoint = point->value(curFile);
			int tempRadius = radius->value(curFile);
			QMessageBox::information(0, QString(tr("%1")).arg(curFile.section('/', -1)), QString(tr("General information about photo: Size: [%1 x %2] Center: [%3, %4], Radius: [%5]")).arg(image.width()).arg(image.height()).arg(tempPoint.x()).arg(tempPoint.y()).arg(tempRadius));
		}
		//else 
		//	QMessageBox::information(0, QString(tr("%1")).arg(curFile.section('/', -1)), QString(tr("General information about photo: Size: [%2 x %3]")).arg(image.width()).arg(image.height()));
    }
}

void MdiChild::wheelEvent(QWheelEvent *e)
{
    if(e->modifiers() == Qt::ControlModifier)           //modyfikator ctrl
    {
        if(e->delta() > 0) zoomIn();                    //metoda delta zwraca wartość liczbową o ile zostało obrócone kólko myszy
        else if(e->delta() < 0) zoomOut();
    }
    else parent()->event(e);                            //jeśli nie ma ctrl to zostanie zdarzenie przekazane do rodzica (obiekt klasy QScrollArea)
}

void MdiChild::zoomIn()
{
    QString tekst = findImage();
    foreach(QString text, dict->keys())
    {
        if(text.section('/', -1) == tekst)
        {
            zoom *= 1.25;
            if(zoom >= 8.0f) zoom = 8.0f;
            setFixedSize(dict->value(text).size()*zoom);
            update();
        }
    }
}

void MdiChild::zoomOut()
{
    QString tekst = findImage();
    foreach(QString text, dict->keys())
    {
        if(text.section('/', -1) == tekst)
        {
            zoom /= 1.25;
            if(zoom <= 0.1f) zoom = 0.1f;
            setFixedSize(dict->value(text).size()*zoom);
            update();
        }
    }
}

void MdiChild::zoomReset()
{
    zoom = 1.0f; //f - wynik w systemie dziesiętnym(specyfikator)
    setFixedSize(image.size()*zoom);
    update();
}

void MdiChild::normalSize()
{
    QString tekst = findImage();
    foreach(QString text, dict->keys())
    {
        if(text.section('/', -1) == tekst)
        {
            zoom = 1.0f; //f - wynik w systemie dziesiętnym(specyfikator)
            setFixedSize(dict->value(text).size()*zoom);
            update();
        }
    }
}

QString MdiChild::findImage()
{
    QString tekst;
    foreach(QString text, action->text())
    {
        if(action->isChecked() == true)
            tekst += text;
    }
    return tekst;
}
