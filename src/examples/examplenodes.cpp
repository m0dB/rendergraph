#include "examplenodes.h"

#include <QColor>
#include <QVector4D>

using namespace rendergraph;

ExampleNode1::ExampleNode1() {
    setMaterial(std::make_unique<EndOfTrackMaterial>());
    setGeometry(std::make_unique<Geometry>(EndOfTrackMaterial::attributes(), 4));

    geometry().setAttributeValues(0, positionArray, 4);
    geometry().setAttributeValues(1, horizontalGradientArray, 4);

    QColor color("red");
    material().setUniform(0, QVector4D{color.redF(), color.greenF(), color.blueF(), color.alphaF()});
}

ExampleNode2::ExampleNode2() {
    auto* m = new EndOfTrackMaterial;

    setMaterial(std::make_unique<EndOfTrackMaterial>());
    setGeometry(std::make_unique<Geometry>(EndOfTrackMaterial::attributes(), 4));

    geometry().setAttributeValues(0, positionArray, 4);
    geometry().setAttributeValues(1, horizontalGradientArray, 4);

    QColor color("blue");
    material().setUniform(0, QVector4D{color.redF(), color.greenF(), color.blueF(), color.alphaF()});
}
