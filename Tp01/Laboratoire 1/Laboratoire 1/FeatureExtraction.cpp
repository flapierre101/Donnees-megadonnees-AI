#include "FeatureExtraction.h"
#include <Qdebug>
#include <Array>


std::array<float,3> FeatureExtraction::scanImage(QImage const& image)
{
    QColor opaqueWhite(255, 255, 255, 255); 
    QColor opaqueBlack(0, 0, 0, 255);

    int derniereCouleur, couleur, aire, perim, minX, maxX, minY, maxY, centralX, centralY;
    couleur = derniereCouleur = aire = perim = NULL;
    maxX = maxY = 0;
    minX = minY = 250;

    std::array<float, 3> reponse;
    
    QVector<QPoint> vectorPoint, vectorCoins;
    QImage mImage = image;

    int sommeX{}, sommeY{};


    for (int x{}; x < mImage.width(); ++x) {              // début de l'itération sur chaque pixel de l'image

        for (int y{}; y < mImage.height(); ++y) {

            if (mImage.pixelColor(x, y) == opaqueWhite) { // acc�s � un pixel en lecture monImage.setPi xelCol or(x, y, o pa queRed); // acc�s � un pixel en �criture
                couleur = 1;
                ++aire;

                sommeX += x;
                sommeY += y;

                if (x > maxX)                           // pour obtenir les minimum XY et maximum XY, comparaison des positions de chaque pixel 
                    maxX = x;
                if (x < minX)
                    minX = x;
                if (y > maxY)
                    maxY = y;
                if (y < minY)
                    minY = y;

                if (!derniereCouleur) {               // lorsque la couleur n'est pas la même que la dernière, il s'agit d'un pixel situé sur le périmètre
                    ++perim;
                    
                    if (!vectorPoint.contains(QPoint(x, y)))      // vérification si le point existe déjà dans le vecteur contenant les points du périmètre
                        vectorPoint.append(QPoint(x, y));

                }
                else {
                    if (mImage.pixelColor(x - 1, y) == opaqueBlack || mImage.pixelColor(x + 1, y) == opaqueBlack)       // si pixels noirs à ces positions spécifiques, il s'agit d'un pixel situé sur périmètre
                    {
                        if (!vectorPoint.contains(QPoint(x, y)))
                            vectorPoint.append(QPoint(x, y));

                        perim++;
                    }
                }

            }
            else {
                couleur = 0;            // si pixel central est noir et dernière couleur est la même
                if (derniereCouleur) {
                    if (!vectorPoint.contains(QPoint(x, y - 1)))
                        vectorPoint.append(QPoint(x, y - 1));             // y - 1 pour seulement prendre les points incluants pixels blancs

                    ++perim;
                }

            }
            derniereCouleur = couleur;

        }
    }
    centralX = minX + (maxX - minX) / 2;
    centralY = minY + (maxY - minY) / 2;

    int x, y, counterBlanc;

    float cX{ sommeX / (float)aire };           // point central X selon la somme des pixels blancs X et l'aire de la forme
    float cY{ sommeY / (float)aire };           // point central Y selon la somme des pixels blancs X et l'aire de la forme


    int blancFort{}, noirFort{};

    float distanceMin, distanceMax, nbCoins;
    distanceMin = 250;
    distanceMax = nbCoins = 0;

    for (int i{}; i < vectorPoint.size(); ++i) {           
        x = vectorPoint[i].x();

        y = vectorPoint[i].y();


        float distanceTemp = calculDistance(cX, cY, x, y);          // calcul de la distance entre point central et points du périmètre pour obtenir distance Max et Min

        if (distanceTemp > distanceMax)
            distanceMax = distanceTemp;
        if (distanceTemp < distanceMin)
            distanceMin = distanceTemp;

        counterBlanc = 0;

        if (mImage.pixelColor(x, y) == opaqueWhite) {                 // on compte le nombre de voisins qui sont blancs
            if (mImage.pixelColor(x - 1, y - 1) == opaqueWhite)
                counterBlanc++;
            if (mImage.pixelColor(x, y - 1) == opaqueWhite)
                counterBlanc++;
            if (mImage.pixelColor(x + 1, y - 1) == opaqueWhite)
                counterBlanc++;
            if (mImage.pixelColor(x + 1, y) == opaqueWhite)
                counterBlanc++;
            if (mImage.pixelColor(x - 1, y) == opaqueWhite)
                counterBlanc++;
            if (mImage.pixelColor(x - 1, y + 1) == opaqueWhite)
                counterBlanc++;
            if (mImage.pixelColor(x, y + 1) == opaqueWhite)
                counterBlanc++;
            if (mImage.pixelColor(x + 1, y + 1) == opaqueWhite)
                counterBlanc++;
        }

        



        if (counterBlanc <= 3) {              // coin possible si 3 pixels blancs ou moins 
            bool voisinCoin = false;
            // Vérifie si pixel voisin est déjà considéré comme un point.
            if (vectorCoins.contains(QPoint(x - 1, y - 1)) || vectorCoins.contains(QPoint(x, y - 1))       
                || vectorCoins.contains(QPoint(x + 1, y - 1)) || vectorCoins.contains(QPoint(x + 1, y))
                || vectorCoins.contains(QPoint(x - 1, y)) || vectorCoins.contains(QPoint(x - 1, y + 1))
                || vectorCoins.contains(QPoint(x, y + 1)) || vectorCoins.contains(QPoint(x + 1, y + 1)))
                voisinCoin = true;

            if (!voisinCoin) {
                vectorCoins.append(QPoint(x, y));           // si aucun pixel voisin est un coin, on conclut que le pixel central est un coin
                ++nbCoins;
            }

        }

    }      

    

    float varianceDist = distanceMin / distanceMax; // pas besoin de plus de normalisation!

    float complexite = (((pow(perim, 2)) / (4 * 3.1416 * aire)) / 6.5); // 6.5 pour la normalisation

    nbCoins = nbCoins / 10; // pour normalisation


    reponse[0] =varianceDist;
    reponse[1] = complexite;
    reponse[2] =nbCoins;

    return reponse;

}

float FeatureExtraction::calculDistance(float x1, float y1, int x2, int y2)
{
    float dx = pow(abs((float)x2 - (float)x1), 2);
    float dy = pow(abs((float)y2 - (float)y1), 2);

    float distance = sqrt(dx + dy);

    return distance;
}