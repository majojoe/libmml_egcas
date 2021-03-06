#include "formulaview.h"
#include "eg_mml_document.h"

#include <qdebug.h>
#include <qevent.h>
#include <qpainter.h>

FormulaView::FormulaView( QWidget *parent ):
    QWidget( parent ),
    d_fontSize( 8 ),
    d_transformation( false ),
    d_scale( false ),
    d_rotation( 0 ),
    d_idRects( false )
{
}

QString FormulaView::formula() const
{
    return d_formula;
}

void FormulaView::setFormula( const QString &formula )
{
    d_formula = formula;
    update();
}

void FormulaView::setFontSize( const qreal &fontSize )
{
    d_fontSize = fontSize;
    update();
}

void FormulaView::setTransformation( const bool &transformation )
{
    d_transformation = transformation;
    update();
}

void FormulaView::setScale( const bool &scale )
{
    d_scale = scale;
    update();
}

void FormulaView::setRotation( const qreal &rotation )
{
    d_rotation = rotation;
    update();
}

void FormulaView::setDrawFrames( const bool &drawFrames )
{
    d_drawFrames = drawFrames;
    update();
}

void FormulaView::setColors( const bool &colors )
{
    d_colors = colors;
    update();
}

void FormulaView::setIdRects( const bool &idRects )
{
    d_idRects = idRects;
    update();
}


void FormulaView::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    painter.setClipRegion( event->region() );

    painter.fillRect( event->rect(), Qt::white );

    renderFormula( &painter );
}

void FormulaView::paintIdRects(EgMathMLDocument *doc, QPainter *painter ) const
{
        QVector<EgRenderingPosition> renderingPosition = doc->getRenderingPositions();

        EgRenderingPosition position;
        Qt::GlobalColor color = Qt::gray;
        static int i = 1;
        foreach (position, renderingPosition) {
                if (position.m_subPos == 0) {
                        painter->setPen(Qt::darkGray);
                        painter->drawRect(position.m_itemRect);
                } else {
                        painter->setPen(color);
                        painter->drawRect(position.m_itemRect);
                }

                if (color == Qt::darkYellow)
                        color = Qt::gray;
                else
                        color = static_cast<Qt::GlobalColor>(static_cast<int>(color) + 1);
        }
}

void FormulaView::renderFormula( QPainter *painter ) const
{
    EgMathMLDocument doc;
    doc.setBaseFontPixelSize( d_fontSize );
    doc.setDrawFrames( d_drawFrames );
    doc.setContent( d_formula );
    if ( d_colors )
    {
        doc.setBackgroundColor( Qt::darkCyan );
        doc.setForegroundColor( Qt::yellow );
    }
    else
    {
        doc.setBackgroundColor( Qt::white );
        doc.setForegroundColor( Qt::black );
    }

    QRectF docRect;
    docRect.setSize( doc.size() );
    docRect.moveCenter( rect().center() );

    if ( d_transformation )
    {
        const double scaleF = d_scale ? 2.0 : 1.0;

        painter->save();

        painter->translate( docRect.center() );
        painter->rotate( d_rotation );
        painter->scale( scaleF, scaleF );
        painter->translate( docRect.topLeft() - docRect.center() );
        doc.paint( painter, QPointF( 0.0, 0.0 ) );

        if (d_idRects) {
                paintIdRects(&doc, painter);
        }

        painter->restore();
    }
    else
    {
        painter->save();

        doc.paint( painter, docRect.topLeft() );

        if (d_idRects) {
                paintIdRects(&doc, painter);
        }

        painter->restore();
    }
}

