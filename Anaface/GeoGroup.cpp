
#include "GeoGroup.h"
#include "AnafaceUI.h"


GeoGroup::GeoGroup(ID2D1RenderTarget * rt)
{
	secondColor = false;
	useRadial = false;
	gradients[0].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradients[0].position = 0.0f;
	gradients[1].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradients[1].position = 1.0f;

	crunch = true;
}

GeoGroup::~GeoGroup()
{
	if (brush.Get())
	{
		brush.Nullify();
	}
}

bool GeoGroup::onCreate(D2D1_RECT_F r)
{
	loc = r;
	TrecPointer<TString> valpoint = attributes.retrieveEntry(TString(L"|FillBrush"));
	if(valpoint.Get())
	{
		color1 = convertStringToD2DColor(valpoint.Get());
		for (int c = 0; c < children.Count();c++)
		{
			children.ElementAt(c)->setFill(true);
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Brush2"));
	if (valpoint.Get())
	{
		color2 = convertStringToD2DColor(valpoint.Get());
		secondColor = true;
	}

	valpoint = attributes.retrieveEntry(TString(L"|StrokeBrush"));
	if (valpoint.Get())
	{
		color1 = convertStringToD2DColor(valpoint.Get());
		for (int c = 0; c < children.Count();c++)
		{
			children.ElementAt(c)->setFill(false);
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Thickness"));
	float floaty = 0.0;
	if (valpoint.Get() && !valpoint->ConvertToFloat(floaty))
	{
		for (int c = 0;c < children.Count();c++)
		{
			children.ElementAt(c)->setThickness(floaty);
		}
	}

	valpoint = attributes.retrieveEntry(TString(L"|Gradient1"));
	if (valpoint.Get() && !valpoint->ConvertToFloat(floaty))
		gradients[0].position = floaty;

	valpoint = attributes.retrieveEntry(TString(L"|Gradient2"));
	if (valpoint.Get() && !valpoint->ConvertToFloat(floaty))
		gradients[1].position = floaty;

	valpoint = attributes.retrieveEntry(TString(L"|GraientMode"));
	if (valpoint.Get() && !valpoint->Compare(L"Radial"))
		useRadial = true;


	valpoint = attributes.retrieveEntry(TString(L"|CompressGeometry"));
	if (valpoint.Get() && !valpoint->Compare(L"False"))
		crunch = false;

	ResetBrush();


	for (int c = 0; c < children.Count();c++)
	{
		children.ElementAt(c)->onCreate(r, crunch);
	}

	return true;
}

void GeoGroup::onDraw()
{
	if (!brush.Get() || !renderTarget.Get())
		return;

	for (int c = 0; c < children.Count();c++)
	{
		children.ElementAt(c)->onDraw(brush.Get(), renderTarget.Get());
	}
}

bool GeoGroup::addAttribute(WCHAR * attr, TrecPointer<TString> value)
{
	if(!attr || !value.Get())
	return false;
	attributes.addEntry(TString(attr), value);
	return true;
}

void GeoGroup::SetNewRenderTarget(TrecComPointer<ID2D1RenderTarget> rt)
{
	if (!rt.Get())
		throw L"Error! Expected Render Target to be initialized!";

	renderTarget = rt;

	ResetBrush();
}

void GeoGroup::addGeometry(TrecPointer<TDrawGeometry> tg)
{
	if (tg.Get())
	{
		children.Add(tg);
	}
}

void GeoGroup::ResetBrush()
{
	if (!secondColor)
	{
		TrecComPointer<ID2D1SolidColorBrush>::TrecComHolder solBrush;
		renderTarget->CreateSolidColorBrush(color1, solBrush.GetPointerAddress());
		brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1SolidColorBrush>(solBrush);
	}
	else
	{
		TrecComPointer<ID2D1GradientStopCollection>::TrecComHolder gsc;
		renderTarget->CreateGradientStopCollection(gradients, 2, gsc.GetPointerAddress());
		stopColl = gsc.Extract();
		if (useRadial)
		{
			TrecComPointer<ID2D1RadialGradientBrush>::TrecComHolder radBrush;
			renderTarget->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
				D2D1::Point2F(loc.left, loc.top),
				D2D1::Point2F(loc.right, loc.bottom),
				loc.right - loc.left, loc.bottom - loc.top),
				stopColl.Get(), radBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1RadialGradientBrush>(radBrush);
		}
		else
		{
			TrecComPointer<ID2D1LinearGradientBrush>::TrecComHolder linBrush;
			renderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(
				D2D1::Point2F(loc.left, loc.top),
				D2D1::Point2F(loc.right, loc.bottom)),
				stopColl.Get(), linBrush.GetPointerAddress());
			brush = TrecPointerKey::GetComPointer<ID2D1Brush, ID2D1LinearGradientBrush>(linBrush);
		}

	}
}

