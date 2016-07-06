#ifndef BUFOR_H
#define BUFOR_H

#include <QMap>
#include <QHash>
#include <QString>
#include <QImage>
#include <QFile>

//! Klasa CBufor
/*!
 * Odpowiada za składowanie danych
 */

class CBufor: public QObject
{

    Q_OBJECT

    //! Deklaracja zaprzyjaźnienia się z MdiChild
    /*!
     *  MdiChild operuje bezpośrednio na kontenerze.
     */
    friend class MdiChild;

    //! Obraz wyświetlany podczas braku zadanego.
    QImage  defaultImage;

public:
    //! Konstruktor
    CBufor();

    //! Obiekt klasy QHash będący rdzeniem CBufor.
    QHash<QString,QImage> container;
	QHash<QString, QPoint> point_container;
	QHash<QString, int> radius_container;
    //! Dodaje obrazy do bufora
    /*!
     * Obsługiwane formaty: BMP, GIF, JPG, JPEG, PNG, PBM, PGM, PPM, XBM, XPM
     *
     * \param path klucz będący adresem obrazu na dysku
     */
    void    addImage(QString path);

    //! Odpowiada za wyłuskanie obrazu z bufora
    /*!
     * \param path klucz będący adresem obrazu na dysku
     * \return QImage zwraca obraz klasy QImage
     */
    QImage  getImageByPath(QString path);

    //! Zwraca liczbę obrazów w buforze
    /*!
     * \return int liczba obrazów w buforze
     */
    int     getNumberOfImages();

    //! Czyści bufor
    void    removeAll();

public slots:

    //! Usuwa obraz
    /*!
     * \param path klucz będący adresem obrazu na dysku
     * \return int zwraca liczbę usuniętych obrazów,
     *         nie przewiduję innej opcji niż 0 (brak obrazu) i 1 (był obraz)
     */
    int     removeImageByPath(QString path);

private:


signals:
    //! Sygnał informujący o pomyślnym dodaniu obrazu
    void    imageWasAdded();

    //! Sygnał informujący o pomyślnym usunięciu obrazu
    void    imageWasRemoved();

};

#endif // BUFOR_H
