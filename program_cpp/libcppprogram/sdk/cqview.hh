#pragma once

#include "cqresponder.hh"
#include "cqgeometry.hh"

cq_class(cqView, cqResponder) {
    
    cqView();
    
    virtual void setFrame(cqRect frame);
    virtual cqRect frame();
    virtual cqRect bounds();
    
    virtual cqView::Ref superview();
    virtual const std::vector<cqView::Ref> &subviews();
    virtual void addSubview(cqView::Ref subview);
    virtual void removeFromSuperview();
    
    virtual cqView::Ref hitTest(cqPoint point, cqEvent::Ref event);
    virtual bool pointInside(cqPoint point, cqEvent::Ref event);
    
    virtual void setViewDelegate(cqResponder::Ref viewDelegate);
    virtual cqResponder::Ref viewDelegate();
    
    cqResponder::Ref nextResponder() override;
};
