#ifndef MDICHILD_H
#define MDICHILD_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QScrollArea>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QAction>
#include <QList>
#include <QMap>
#include <QString>

#include "CBufor.h"
#include "CAlgorithmPanel.h"

//! Klasa MdiChild
/*!
 * Odpowiada za manipulowanie obrazem
 */
class MdiChild : public QWidget
{
    Q_OBJECT
public:
    //! Konstruktor
    /*!
     * Tworzony akcję do menu window oraz włącza
     * obsługę zdarzeń myszy i usuwa podokna
     * \param parent wskaźnik do rodzica
     */
    explicit MdiChild(CBufor *bufor,  CAlgorithmPanel *algorithmPanel, QWidget *parent = 0);
    //! Zapisuje obraz do zmiennej image
    void setImage(const QImage & im);
    //! Zapisuje obraz do zmiennej dict oraz nazwę do zmiennej curFile
    void setImage(const QString &name);
    /*!
     * \brief windowMenuAction
     * \return zwraca wskaźnik na QAction
     */
    QAction *windowMenuAction() const { return action; }

    QString curFile;
    /*!
     * \brief currentFile
     * \return zwraca curFile
     */
    QString currentFile() { return curFile; }

private slots:
    //! Zmniejsza obraz
    /*!
     * Wykorzystuje funkcję findChild() do znalezienia nazwy aktywnego podokna, a
     * następnie szuka obrazu w buforze dict.
     *\sa findChild(), update()
     */
    void zoomOut();
    //! Resetuje powiększenie obrazu
    void zoomReset();
    //! Powiększa obraz
    /*!
     * Wykorzystuje funkcję findChild() do znalezienia nazwy aktywnego podokna, a
     * następnie szuka obrazu w buforze dict.
     *\sa findChild(), update()
     */
    void zoomIn();
    //! Resetuje powiększenie obrazu
    /*!
     * Wykorzystuje funkcję findChild() do znalezienia nazwy aktywnego podokna, a
     * następnie szuka obrazu w buforze dict.
     *\sa findChild(), update()
     */
    void normalSize();

signals:
    void info(QString);
    void setActiveFilePath(QString);
    void activeFilePath();

private:
    float zoom;
    QAction *action;
    QImage image;

    QHash<QString, QImage> *dict;
	QHash<QString, QPoint> *point;
	QHash<QString, int>	   *radius;
    CAlgorithmPanel        *algoPanel;

public:
    //! Obłsuguje ruch myszą
    /*!
     * W konstruktorze włączyliśmy opcję mouseTracking więc zdarzenie jest wywoływane
     * zawsze. Metoda odczytuje współrzędne myszy i wyświetla w pasku stanu wartości RGB
     */
    void mouseMoveEvent(QMouseEvent *);
    //! Obłsuguje kliknięcie przycisku myszy
    /*!
     * Po naciśnięciu na widget i puszczeniu przycisku zostaje wyświetlona nazwa i rozmiar widgetu.
     */
    void mouseReleaseEvent(QMouseEvent *);
    //! Obłsuguje scroll myszy
    /*!
     * Metoda delta zwraca wartość liczbową o ile zostało obrócone kólko myszy i jeśli jest
     * wciśnięty ctrl to wlącza sloty zoomIn() i zoomOut(). Jeśli nie ma ctrl to zostanie
     * zdarzenie przekazane do rodzica (obiekt klasy QScrollArea)
     */
    void wheelEvent(QWheelEvent *);
    //! Rysuje obraz na widgecie
    /*!
     * przeskalowuje układ współrzednych paintera, skaluje jednocześnie
     * rysowany w nastepnej linii obraz.
     */
    void paintEvent(QPaintEvent *);
    //! znajduje nazwę aktywnego widgeta
    QString findImage();
};

#endif
