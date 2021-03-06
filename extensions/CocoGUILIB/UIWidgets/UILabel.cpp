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

#include "UILabel.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "iconv/iconv.h"
#endif

NS_CC_EXT_BEGIN

UILabel::UILabel():
m_bTouchScaleChangeAble(false),
m_nGravity(LabelGravityCenter),
m_sFontName("Thonburi"),
m_nFontSize(10),
m_fOnSelectedScaleOffset(0.5),
m_fNormalScaleValue(1),
m_pRenderLabel(NULL)
{
    m_WidgetName = WIDGET_LABEL;
}

UILabel::~UILabel()
{
    
}

UILabel* UILabel::create()
{
    UILabel* widget = new UILabel();
    if (widget && widget->init())
    {
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool UILabel::init()
{
    if (UIWidget::init())
    {
        this->setPressState(WidgetStateNormal);
        return true;
    }
    return false;
}

void UILabel::initNodes()
{
    UIWidget::initNodes();
    this->m_pRenderLabel = cocos2d::CCLabelTTF::create();
    this->m_pCCRenderNode->addChild(m_pRenderLabel);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    static const char* UTF8ToGBK(const char *strChar) 
    {
        iconv_t iconvH;  
        iconvH = iconv_open("gb2312","utf-8"); 
        if (iconvH == 0) 
        {  
            return NULL; 
        }  

        size_t strLength = strlen(strChar); 
        size_t outLength = strLength;  

        size_t copyLength = outLength; 

        char* outbuf =  new char[outLength + 1];  
        char* pBuff = outbuf;  
        memset( outbuf, 0, outLength + 1); 

        if (-1 == iconv(iconvH, &strChar, &strLength, &outbuf, &outLength)) 
        {  
            iconv_close(iconvH); 
            return NULL; 
        }  
        iconv_close(iconvH); 
        return pBuff; 
    }
#endif

void UILabel::setText(const char* text)
{
    std::string strText(text);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	const char *des = UTF8ToGBK(strText.c_str());
	strText.assign(des);
	CC_SAFE_DELETE(des);
#endif
    m_pRenderLabel->setString(strText.c_str());
}

const char* UILabel::getStringValue()
{
    return m_pRenderLabel->getString();
}

int UILabel::getStringLength()
{
    const char* str = m_pRenderLabel->getString();
    return strlen(str);
}

void UILabel::setFontSize(int size)
{
    m_pRenderLabel->setFontSize(size);
}

void UILabel::setFontName(const char* name)
{
    m_pRenderLabel->setFontName(name);
}

void UILabel::setTouchScaleChangeAble(bool able)
{
    this->m_bTouchScaleChangeAble = able;
    this->m_fNormalScaleValue = this->getScale();
}

bool UILabel::getTouchScaleChangeAble()
{
    return this->m_bTouchScaleChangeAble;
}

void UILabel::onPressStateChangedToNormal()
{
    if (!this->m_bTouchScaleChangeAble){
        return;
    }
    this->clickScale(this->m_fNormalScaleValue);
}

void UILabel::onPressStateChangedToPressed()
{
    if (!this->m_bTouchScaleChangeAble){
        return;
    }
    this->clickScale(this->m_fNormalScaleValue + this->m_fOnSelectedScaleOffset);
}

void UILabel::onPressStateChangedToDisabled()
{
    
}

void UILabel::clickScale(float scale)
{
    this->m_pCCRenderNode->setScale(scale);
}

void UILabel::setFlipX(bool flipX)
{
    m_pRenderLabel->setFlipX(flipX);
}

void UILabel::setFlipY(bool flipY)
{
    m_pRenderLabel->setFlipY(flipY);
}

bool UILabel::isFlipX()
{
    return m_pRenderLabel->isFlipX();
}

bool UILabel::isFlipY()
{
    return m_pRenderLabel->isFlipY();
}

void UILabel::setGravity(LabelGravity gravity)
{
    this->m_nGravity = gravity;
    switch (this->m_nGravity)
    {
        case LabelGravityCenter:
            m_pRenderLabel->setAnchorPoint(ccp(0.5, 0.5));
            break;
        case LabelGravityLelf:
            m_pRenderLabel->setAnchorPoint(ccp(0.0, 0.5));
            break;
        case LabelGravityRight:
            m_pRenderLabel->setAnchorPoint(ccp(1.0, 0.5));
            break;
        default:
            break;
    }
}

void UILabel::adaptSize(float xProportion, float yProportion)
{
    float res = xProportion > yProportion ? xProportion : yProportion;
    m_pRenderLabel->setFontSize(m_pRenderLabel->getFontSize()*res);
}

CCNode* UILabel::getValidNode()
{
    return this->m_pRenderLabel;
}

void UILabel::setAnchorPoint(const cocos2d::CCPoint &pt)
{
    UIWidget::setAnchorPoint(pt);
    this->m_pRenderLabel->setAnchorPoint(pt);
}

NS_CC_EXT_END