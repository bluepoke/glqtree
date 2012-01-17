#include "graphwidget.h"

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent)
{
    // some minimum area to display the graph on
    this->setMinimumSize(2 * MARGIN + WIDTH, 2 * MARGIN + HEIGHT);
    // grab the table to gather the data
    table = (QTableWidget*)parent;
}

void GraphWidget::paintEvent(QPaintEvent *event) {
    // different pen styles
    QPen penBlack(Qt::black, 1, Qt::SolidLine);
    QPen penRed(Qt::red, 1, Qt::SolidLine);
    QPen penBlue(Qt::blue, 1, Qt::SolidLine);
    // set a font to match the margins to write on
    QFont font = this->font();
    font.setPixelSize(MARGIN - 5);

    // the painter to paint with
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(font);

    // where are the relevant columns?
    int valueColumnPos = -1;
    int probabilityColumnPos = -1;

    if (probabilityColumn && valueColumn) {
        valueColumnPos = table->columnCount() - 2;
        probabilityColumnPos = table->columnCount() - 3;
    }
    else if (!probabilityColumn && valueColumn ) {
        valueColumnPos = table->columnCount() - 2;
    }
    else if (probabilityColumn && !valueColumn) {
        probabilityColumnPos = table->columnCount() - 2;
    }
    // the data rows minus the one to add new data
    const int dataRowCount = table->rowCount() - 1;

    // paint the coordinate system with a probability axis
    if (probabilityColumn && valueColumn) {
        static const QPointF ptsFrame[4] = {
            QPointF(MARGIN, MARGIN),
            QPointF(MARGIN, MARGIN + HEIGHT),
            QPointF(MARGIN + WIDTH, MARGIN + HEIGHT),
            QPointF(MARGIN + WIDTH, MARGIN)
        };
        painter.setPen(penBlack);
        painter.drawPolyline(ptsFrame, 4);
    }
    // or one without
    else {
        static const QPointF ptsFrame[3] = {
            QPointF(MARGIN, MARGIN),
            QPointF(MARGIN, MARGIN + HEIGHT),
            QPointF(MARGIN + WIDTH, MARGIN + HEIGHT)
        };
        painter.setPen(penBlack);
        painter.drawPolyline(ptsFrame, 3);
    }

    // arrays to hold the data
    int values[dataRowCount];
    int ages[dataRowCount];
    double probabilities[dataRowCount];

    // fill them with the data
    for (int i= 0; i < dataRowCount; i++) {
        ages[i] = ((QLabel*)table->cellWidget(i, 0))->text().toInt();
        // if there are respective columns ...
        if (probabilityColumn)
            probabilities[i] = ((QDoubleSpinBox*)table->cellWidget(i, probabilityColumn))->value();
        if (valueColumn)
            values[i] = ((QSpinBox*)table->cellWidget(i, valueColumn))->value();
    }

    // find the maximum values and calculate a stepping
    int maxValue = *max_element(values, values + dataRowCount);
    int maxAge = ages[dataRowCount - 1];
    double valueQuot = (double)HEIGHT / (double)maxValue;
    double agesQuot = (double)HEIGHT / (double)maxAge;

    // paint lines for all the values if there are any
    if (valueColumn) {
        QPainterPath valueGraph;
        valueGraph.moveTo(MARGIN + ages[0] * agesQuot, MARGIN + HEIGHT - (int)(values[0] * valueQuot));
        for (int i = 1; i < dataRowCount; i++) {
            valueGraph.lineTo(MARGIN + ages[i] * agesQuot, MARGIN + HEIGHT - (int)(values[i] * valueQuot));
        }
        painter.setPen(penRed);
        painter.drawPath(valueGraph);
    }

    // paint lines for all the probabilities if there are any
    if (probabilityColumn) {
        QPainterPath probabilitiesGraph;
        probabilitiesGraph.moveTo(MARGIN + ages[0] * agesQuot, MARGIN + HEIGHT - (int)(probabilities[0] * HEIGHT));
        for (int i = 1; i < dataRowCount; i++) {
            probabilitiesGraph.lineTo(MARGIN + ages[i] * agesQuot, MARGIN + HEIGHT - (int)(probabilities[i] * HEIGHT));
        }
        painter.setPen(penBlue);
        painter.drawPath(probabilitiesGraph);
    }

    // put some text underneath
    painter.setPen(penBlack);
    painter.drawText(QRect(MARGIN, MARGIN + HEIGHT, WIDTH, MARGIN), Qt::AlignCenter, QString("Age [0 .. " + QString::number(maxAge) + "]"));

    // and on the left axis
    if (valueColumn) {
        painter.setPen(penRed);
        painter.drawText(QRect(0, 0, MARGIN, MARGIN), Qt::AlignCenter, QString::number(maxValue));
        painter.drawText(QRect(0, MARGIN + HEIGHT, MARGIN, MARGIN), Qt::AlignCenter, "0");

        painter.save();
        painter.rotate(-90);
        painter.translate(-1 * (2 * MARGIN + HEIGHT), 0);
        painter.drawText(QRect(MARGIN, 0, HEIGHT, MARGIN), Qt::AlignCenter, "Value");
    }
    else if (probabilityColumn) {
        painter.setPen(penBlue);
        painter.drawText(QRect(0, 0, MARGIN, MARGIN), Qt::AlignCenter, "1.0");
        painter.drawText(QRect(0, MARGIN + HEIGHT, MARGIN, MARGIN), Qt::AlignCenter, "0");

        painter.save();
        painter.rotate(-90);
        painter.translate(-1 * (2 * MARGIN + HEIGHT), 0);
        painter.drawText(QRect(MARGIN, 0, HEIGHT, MARGIN), Qt::AlignCenter, "Probability");
    }

    // if necessary, put some more text on the right axis
    if (probabilityColumn && valueColumn) {
        painter.setPen(penBlue);
        painter.translate(0, WIDTH + MARGIN);
        painter.drawText(QRect(MARGIN, 0, HEIGHT, MARGIN), Qt::AlignCenter, "Probability");

        painter.restore();
        painter.setPen(penBlue);
        painter.drawText(QRect(MARGIN + WIDTH, 0, MARGIN, MARGIN), Qt::AlignCenter, "1.0");
        painter.drawText(QRect(MARGIN + WIDTH, MARGIN + HEIGHT, MARGIN, MARGIN), Qt::AlignCenter, "0.0");
    }
    else {
        painter.restore();
    }
}
