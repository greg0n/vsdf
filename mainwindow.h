#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QtConcurrent>
#include <QDialogButtonBox>

#include "CBufor.h"
#include "CAlgorithmPanel.h"

//! Katalgo z ikonkami
#define ICON(x) "/home/konrad/workspace/vsdf/ikonki/"#x

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QListWidget;
class QScrollArea;
class QActionGroup;
class QString;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

//! Klasa MainWindow
/*!
 * Odpowiada za stworzenie głównego okna aplikacji MDI
 * oraz podokien.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    CBufor *myBufor;
    CAlgorithmPanel *algorithmPanel;

public:

    //! Konstruktor
    /*!
     * Tworzy wskaźnik na obiekt klasy QMdiArea, wszystkie akcje
     * oraz pasek stanu, narzędzi i menu.
     *\param parent wskaźnik do rodzica
     */

    explicit MainWindow(QWidget *parent = 0);

    //! Destruktor
    ~MainWindow();
    
public slots:

    void updateMdiSubWindow(QString fileName, QString processedImage);

private:
    Ui::MainWindow *ui;
    //CAlgorithmPanel *algorithmPanel();

    QString                 currentFile;
    QFutureWatcher<void>    futureWatcher;
    QStringList             filesList;
    QStringList             filesListIcon;
    int                     numberOfImages;
    void                    addIconToBufferView(QString nameOfImageInBuffer);

private slots:
    //! Wyświetla napis na pasku stanu
    /*!
     * \param str argument typu QString
     */
    void info(QString str);
    //! Otwiera obraz z dysku
    /*!
     * Wybrany obraz przetwarza do formatu QIcon i dodaje do listy
     * QListWidgetItem w celu późniejszego wyświetlenia
     */
    void open();
    //! Aktualizuje stan akcji w window i view menu
    /*!
     * Ustawia aktywność akcji w zależności czy funkcja activeMdiChild()
     * zwróci adres do okna klasy MdiChild (okno musi istnieć!),
     * a także zaznacza aktywne okno w window menu.
     * \sa activeMdiChild(), windowMenuAction()
     */
    void updateMenus();
    //! Uaktywnia okno przekazane jako argument
    /*!
     * \param window wskaźnik na okno
     */
    void setActiveSubWindow(QWidget *window);
    //! Otwiera obraz z bufora w oknie głównym jako podokno.
    /*!
     * Do zmiennej fileName zostaje przypisany adres obrazu, jeśli zostanie
     * on znaleziony przez funkcję findMdiChild, uaktywnia się już otwarte
     * podokno. Jeżeli nie, zostanie stworzony obiekt klasy MdiChild oraz
     * przekazany do niego obraz z nazwą. Następnie obiekt staje się dzieckiem
     * QScrollArea oraz zostaje dodany do QMdiArea. Zostaje wyświetlony,
     * a także dodany do window menu. Na pasku stanu pojawi się napis. Zostaną
     * uaktywnione odpowiednie sloty.
     *\sa findMdiChild(), setImage(), setActiveMdiChild().
     */
    void showItem();
    //! Obsługa menu kontekstowego w buforze
    void showContextMenu();
    //! Usuwa obiekt z bufora oraz z QMdiArea
    void removeItem();
    //! Usuwa wszystkie obiekty z bufora oraz z QMdiArea
    void clearBufor();

    void onOpenImageFinished();
	void HandleItem();
	void save();
	void autoSave();
	void manualSave();

private:
	bool hasItem();
    void createAlgorithmPanel();
    //! Tworzy akcje każdego menu
    void createActions();
    //! Tworzy menu
    void createMenus();
    //! Tworzy pasek narzędzi
    void createToolBars();
    //! Tworzy pasek stanu
    void createStatusBar();
    //! Dokuje bufor w oknie głównym programu
    /*!
     * Uaktywnia także sloty dla bufora.
     *\sa showContextMenu(), showItem()
     */
    void createDockWindow();
    //! Zwraca adres aktywnego podokna
    /*!
     * Wykonuje podwójne dynamiczne rzutowanie, ponieważ obiekt
     * klasy MdiChild został najpierw dzieckiem QScrollArea, a potem
     * QMdiArea.
     * \return wskaźnik na typ MdiChild aktywnego podokna lub 0 w razie niepowodzenia
     */

    void createIconsFromNewlyAddedImages();

    MdiChild *activeMdiChild();
    //! Zwraca adres znalezionego podokna
    /*!
     * QString fileName zostaje obcięte do samej nazwy obrazu
     * np C:\...\obraz.jpg do obraz.jpg. Dzięki funkcji foreach zostaje przeszukana
     * lista podokien QMdiArea. Podwójne dynamiczne rzutowanie ze względów dziedziczenia
     * (klasa, która staje się rodzicem przejmuje funkcje dziecka).
     * \param fileName nazwa szukanego podokna
     * \return adres na typ QMdiSubWindow lub 0 w razie niepowodzenia
     */



    QMdiSubWindow *findMdiChild(const QString &fileName);

    QMdiArea *mdiArea;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QToolBar *fileToolBar;
    QToolBar *viewToolBar;

    QAction *openAct;
	QAction *openAllAct;
	QAction *saveAct;
    QAction *exitAct;
    QAction *removeAllWidget;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomResetAct;
    QAction *fitToWindowAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QActionGroup *windowActionGroup;

	QPushButton *autoButton;
	QPushButton *manualButton;
	
	QDialogButtonBox *buttonBox;

    QListWidget *listImage;
};

#endif
