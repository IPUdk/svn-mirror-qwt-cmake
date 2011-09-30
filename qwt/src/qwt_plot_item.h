/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_ITEM_H
#define QWT_PLOT_ITEM_H

#include "qwt_global.h"
#include "qwt_legend_itemmanager.h"
#include "qwt_text.h"
#include <qrect.h>

class QString;
class QPainter;
class QWidget;
class QwtPlot;
class QwtLegend;
class QwtScaleMap;
class QwtScaleDiv;

/*!
  \brief Base class for items on the plot canvas

  A plot item is "something", that can be painted on the plot canvas,
  or only affects the scales of the plot widget. They can be categorized as:

  - Representator\n
    A "Representator" is an item that represents some sort of data
    on the plot canvas. The different representator classes are organized
    according to the characteristics of the data:
    - QwtPlotMarker
      Represents a point or a horizontal/vertical coordinate
    - QwtPlotCurve
      Represents a series of points
    - QwtPlotSpectrogram ( QwtPlotRasterItem )
      Represents raster data
    - ...

  - Decorators\n
    A "Decorator" is an item, that displays additional information, that
    is not related to any data:
    - QwtPlotGrid
    - QwtPlotScaleItem
    - QwtPlotSvgItem
    - ...

  Depending on the QwtPlotItem::ItemAttribute flags, an item is included
  into autoscaling or has an entry on the legnd.

  Before misusing the existing item classes it might be better to
  implement a new type of plot item
  ( don't implement a watermark as spectrogram ).
  Deriving a new type of QwtPlotItem primarily means to implement
  the YourPlotItem::draw() method.

  \sa The cpuplot example shows the implementation of additional plot items.
*/

class QWT_EXPORT QwtPlotItem: public QwtLegendItemManager
{
public:
    /*!
        \brief Runtime type information

        RttiValues is used to cast plot items, without
        having to enable runtime type information of the compiler.
     */
    enum RttiValues
    {
        //! Unspecific value, that can be used, when it doesn't matter
        Rtti_PlotItem = 0,

        //! For QwtPlotGrid
        Rtti_PlotGrid,

        //! For QwtPlotScaleItem
        Rtti_PlotScale,

        //! For QwtPlotMarker
        Rtti_PlotMarker,

        //! For QwtPlotCurve
        Rtti_PlotCurve,

        //! For QwtPlotSpectroCurve
        Rtti_PlotSpectroCurve,

        //! For QwtPlotIntervalCurve
        Rtti_PlotIntervalCurve,

        //! For QwtPlotHistogram
        Rtti_PlotHistogram,

        //! For QwtPlotSpectrogram
        Rtti_PlotSpectrogram,

        //! For QwtPlotSvgItem
        Rtti_PlotSVG,

        //! For QwtPlotTradingCurve
        Rtti_PlotTradingCurve,

        //! For QwtPlotBarChart
        Rtti_PlotBarChart,
        /*! 
           Values >= Rtti_PlotUserItem are reserved for plot items
           not implemented in the Qwt library.
         */
        Rtti_PlotUserItem = 1000
    };

    /*!
       Plot Item Attributes
       \sa setItemAttribute(), testItemAttribute()
     */
    enum ItemAttribute
    {
        //! The item is represented on the legend.
        Legend = 0x01,

        /*!
         The boundingRect() of the item is included in the
         autoscaling calculation.
         */
        AutoScale = 0x02
    };

    //! Plot Item Attributes
    typedef QFlags<ItemAttribute> ItemAttributes;

    //! Render hints
    enum RenderHint
    {
        //! Enable antialiasing
        RenderAntialiased = 0x1,

        /*!
          Certain painter operations are faster, when using integers
          than floats ( f.e raster paint engine: drawPolyline() ) - even
          if the corresponding paint engine doesn't support floats at all.
          So a plot item might decide to transform coordinates into integers 
          for these paint engines.

          By setting RenderFloats this optimization can be suppressed.

          \note For paint engines, that can use floats ( SVG, PDF ) this
                flag has no effect, because these are rendered always
                as floats.

          \sa QwtPainter::roundingAlignment()
         */
        RenderFloats = 0x2
    };

    //! Render hints
    typedef QFlags<RenderHint> RenderHints;

    explicit QwtPlotItem( const QwtText &title = QwtText() );
    virtual ~QwtPlotItem();

    void attach( QwtPlot *plot );
    void detach();

    QwtPlot *plot() const;

    void setTitle( const QString &title );
    void setTitle( const QwtText &title );
    const QwtText &title() const;

    virtual int rtti() const;

    void setItemAttribute( ItemAttribute, bool on = true );
    bool testItemAttribute( ItemAttribute ) const;

    void setRenderHint( RenderHint, bool on = true );
    bool testRenderHint( RenderHint ) const;

    double z() const;
    void setZ( double z );

    void show();
    void hide();
    virtual void setVisible( bool );
    bool isVisible () const;

    void setAxes( int xAxis, int yAxis );

    void setXAxis( int axis );
    int xAxis() const;

    void setYAxis( int axis );
    int yAxis() const;

    virtual void itemChanged();

    /*!
      \brief Draw the item

      \param painter Painter
      \param xMap Maps x-values into pixel coordinates.
      \param yMap Maps y-values into pixel coordinates.
      \param canvasRect Contents rect of the canvas in painter coordinates
    */
    virtual void draw( QPainter *painter,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect ) const = 0;

    virtual QRectF boundingRect() const;

    virtual void updateLegend( QwtLegend * ) const;
    virtual void updateScaleDiv( 
        const QwtScaleDiv&, const QwtScaleDiv& );

    virtual QWidget *legendItem() const;

    QRectF scaleRect( const QwtScaleMap &, const QwtScaleMap & ) const;
    QRectF paintRect( const QwtScaleMap &, const QwtScaleMap & ) const;

private:
    // Disabled copy constructor and operator=
    QwtPlotItem( const QwtPlotItem & );
    QwtPlotItem &operator=( const QwtPlotItem & );

    class PrivateData;
    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotItem::ItemAttributes )
Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotItem::RenderHints )

#endif
