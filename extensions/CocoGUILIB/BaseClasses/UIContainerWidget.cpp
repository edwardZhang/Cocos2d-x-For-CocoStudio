/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "UIContainerWidget.h"
#include "../Drawable/UIClipAbleLayerColor.h"
#include "../Drawable/UIClipAbleLayerGradient.h"

NS_CC_EXT_BEGIN

#define DYNAMIC_CAST_CLIPLAYERCOLOR dynamic_cast<UIClipAbleLayerColor*>(this->m_pCCRenderNode)
#define DYNAMIC_CAST_CLIPLAYERGRADIENT dynamic_cast<UIClipAbleLayerGradient*>(this->m_pCCRenderNode)
    
UIContainerWidget::UIContainerWidget():
m_fWidth(0.0),
m_fHeight(0.0),
m_bClipAble(false),
m_renderType(RENDER_TYPE_LAYERCOLOR)
{
    this->m_WidgetType = WidgetTypeContainer;
    m_WidgetName = WIDGET_CONTAINERWIDGET;
}

UIContainerWidget::~UIContainerWidget()
{
    
}

UIContainerWidget* UIContainerWidget::create()
{
    UIContainerWidget* widget = new UIContainerWidget();
    if (widget && widget->init()) {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

void UIContainerWidget::setLayoutParameter(/*LayoutParameter * parmeter*/)
{
    
}

bool UIContainerWidget::init()
{
    if (UIWidget::init()) {
        return true;
    }
    return false;
}

void UIContainerWidget::initNodes()
{
    this->m_pCCRenderNode = UIClipAbleLayerColor::create();
}

bool UIContainerWidget::isClippingEnable()
{
    return this->m_bClipAble;
}

bool UIContainerWidget::addChild(UIWidget* child)
{   
    UIWidget::addChild(child);
    bool needSetChildCheckAble = false;
    UIWidget* parent = this;
    while (parent != 0){
        if (parent->isClippingEnable()){
            needSetChildCheckAble = true;
            break;
        }
        parent = parent->getWidgetParent();
    }
    
    if (needSetChildCheckAble){
        child->setNeedCheckVisibleDepandParent(true);
    }
    return true;
}

void UIContainerWidget::setClippingEnable(bool able)
{
    this->m_bClipAble = able;
    switch (m_renderType) {
        case RENDER_TYPE_LAYERCOLOR:
            DYNAMIC_CAST_CLIPLAYERCOLOR->setClipAble(able);
            break;
        case RENDER_TYPE_LAYERGRADIENT:
            DYNAMIC_CAST_CLIPLAYERGRADIENT->setClipAble(able);
            break;
        default:
            break;
    }
    for (int i=0; i<this->m_children->count(); i++) {
        UIWidget* child = (UIWidget*)(this->m_children->objectAtIndex(i));
        child->setNeedCheckVisibleDepandParent(able);
    }
}

void UIContainerWidget::setClipRect(const cocos2d::CCRect &rect)
{
    switch (m_renderType)
    {
        case RENDER_TYPE_LAYERCOLOR:
            DYNAMIC_CAST_CLIPLAYERCOLOR->setClipRect(rect);
            break;
        case RENDER_TYPE_LAYERGRADIENT:
            DYNAMIC_CAST_CLIPLAYERGRADIENT->setClipRect(rect);
            break;
        default:
            break;
    }
}

void UIContainerWidget::updateWidth()
{

}

void UIContainerWidget::updateHeight()
{
    
}

void UIContainerWidget::setSize(const cocos2d::CCSize &size)
{
    switch (m_renderType)
    {
        case RENDER_TYPE_LAYERCOLOR:
            DYNAMIC_CAST_CLIPLAYERCOLOR->setContentSize(size);
            break;
        case RENDER_TYPE_LAYERGRADIENT:
            DYNAMIC_CAST_CLIPLAYERGRADIENT->setContentSize(size);
            break;
        default:
            break;
    }
    this->m_fWidth = size.width;
    this->m_fHeight = size.height;
    this->updateClipSize();
}

float UIContainerWidget::getWidth()
{
    return this->m_fWidth;
}

float UIContainerWidget::getHeight()
{
    return this->m_fHeight;
}

bool UIContainerWidget::hitTest(cocos2d::CCNode *node, cocos2d::CCPoint &pt)
{
    cocos2d::CCPoint nsp = node->convertToNodeSpace(pt);
    cocos2d::CCSize bb = node->getContentSize();
    if (nsp.x >= 0 && nsp.x <= bb.width && nsp.y >= 0 && nsp.y <= bb.height)
    {
        return true;
    }
    return false;
}

void UIContainerWidget::onScaleDirtyChanged()
{
    UIWidget::onScaleDirtyChanged();
    this->updateClipSize();
}

void UIContainerWidget::onScaleXDirtyChanged()
{
    UIWidget::onScaleXDirtyChanged();
    this->updateClipSize();
}

void UIContainerWidget::onScaleYDirtyChanged()
{
    UIWidget::onScaleYDirtyChanged();
    this->updateClipSize();
}

void UIContainerWidget::updateClipSize()
{
    float asx = this->getAbsoluteScaleX();
    float asy = this->getAbsoluteScaleY();
    
    switch (m_renderType)
    {
        case RENDER_TYPE_LAYERCOLOR:
        {
            cocos2d::CCSize size = DYNAMIC_CAST_CLIPLAYERCOLOR->getContentSize();
            DYNAMIC_CAST_CLIPLAYERCOLOR->setClipSize(size.width*asx, size.height*asy);
            break;
        }
        case RENDER_TYPE_LAYERGRADIENT:
        {
            cocos2d::CCSize size = DYNAMIC_CAST_CLIPLAYERGRADIENT->getContentSize();
            DYNAMIC_CAST_CLIPLAYERGRADIENT->setClipSize(size.width*asx, size.height*asy);
            break;
        }
        default:
            break;
    }
}

CCSize UIContainerWidget::getWrapSize() const
{
    for (int i=0; i<this->m_children->count(); i++) {
        UIWidget* child = dynamic_cast<UIWidget*>(m_children->objectAtIndex(i));
        switch (child->getWidgetType()) {
            case WidgetTypeWidget:
                
                break;
            case WidgetTypeContainer:
                break;
            default:
                break;
        }
    }
    return CCSizeZero;
}

NS_CC_EXT_END