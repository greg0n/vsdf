#include "CBufor.h"

#define DEFAULT_IMG_PATH "/home/konrad/workspace/vsdf/ikonki/404.png"

CBufor::CBufor()
{
    //QHash<QString,QImage> container;
}


void CBufor::addImage(QString path)
{
    if(QFile::exists(path))
    {
        QImage loadingImage;

        if(loadingImage.load(path))
        {
            container.insert(path, loadingImage);
            emit imageWasAdded();
        }
    }
}

QImage CBufor::getImageByPath(QString path)
{

    QImage defaultImage;
    defaultImage.load(DEFAULT_IMG_PATH);

    return container.value(path,defaultImage);

}

int CBufor::removeImageByPath(QString path)
{

    return container.remove(path);

    emit imageWasRemoved();
}

int CBufor::getNumberOfImages()
{
    return container.size();
}

void CBufor::removeAll()
{
    container.clear();
}
