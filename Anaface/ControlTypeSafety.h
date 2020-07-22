#pragma once

#include "Anaface.h"
#include <d2d1.h>
#include "GeoGroup.h"
#include <TMap.h>
//#include <TPlayer.h>
/*
class TControl;
class TFlyout;
class TContextMenu;
class TLayout;
class TRadioButton;
class TCheckBox;
class TTextField;
class TComboBox;
class TArena;
class TCanvas;
class TVideo;
class GeoGroup;*/
class _ANAFACE_DLL styleTable : public TObject
{
public:
	TString style;
	TMap<TString> names;
};
/*
namespace ControlTypeSafety
{

	typedef enum _ANAFACE_DLL TControlType
	{
		tc_TControl,
		tc_TLayout,
		tc_TComboBox,
		tc_TRadioButton,
		tc_TCheckBox,
		tc_TTextField,
		tc_TArena,
		tc_TVideo,
		tc_TFlyout,
		tc_TContextMenu,
		tc_TCanvas,
		tc_GeoGroup // non TControl
	}TControlType;

	typedef union _ANAFACE_DLL TControlHolder
	{
		std::shared_ptr<TControl> tc;
		std::shared_ptr<TLayout> tl;
		std::shared_ptr<TComboBox> tcob;
		std::shared_ptr<TRadioButton> trb;
		std::shared_ptr<TCheckBox> tchb;
		std::shared_ptr<TTextField> ttf;
		std::shared_ptr<TArena> ta;
		std::shared_ptr<TVideo> tv;
		std::shared_ptr<TFlyout> tf;
		std::shared_ptr<TContextMenu> tcm;
		std::shared_ptr<TCanvas> tca;
		std::shared_ptr<GeoGroup> geo;
	}TControlHolder;

	typedef struct _ANAFACE_DLL TControlPointer
	{
		TControlType contType;
		TControlHolder pointer;
		std::shared_ptr<TControl> rootType;
	}TControlPointer;


	_ANAFACE_DLL TControlPointer getNewTControl(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewTLayout(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewComboBox(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewRadioButton(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewCheckBox(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewTextField(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta,HWND);
	_ANAFACE_DLL TControlPointer getNewArena(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta, HWND);
	_ANAFACE_DLL TControlPointer getNewVideo(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta, HWND);
	_ANAFACE_DLL TControlPointer getNewFlyout(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewContextMenu(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewCanvas(ID2D1RenderTarget* rt, std::shared_ptr<TArray<styleTable>> ta);
	_ANAFACE_DLL TControlPointer getNewGeoGroup(ID2D1RenderTarget* rt);
	bool _ANAFACE_DLL tcp_onCreate(TControlPointer*, RECT);
	void _ANAFACE_DLL tcp_onDraw(TControlPointer*);
	bool _ANAFACE_DLL tcp_addAttribute(TControlPointer*, WCHAR*, std::shared_ptr<TString>);

	void _ANAFACE_DLL deleteControl(TControlPointer**);
}
*/