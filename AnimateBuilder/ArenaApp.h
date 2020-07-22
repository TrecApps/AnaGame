#pragma once
#include <MiniHandler.h>

class ArenaApp;
typedef void (ArenaApp::* ArenaBuilderEvents)(TrecPointer<TControl> tc, EventArgs ea);


class ArenaApp :
	public MiniHandler
{
public:
	ArenaApp(TrecPointer<TControl> m, TrecPointer<TControl> o, TrecPointer<TControl> e, TrecPointer<TInstance>, TString& arenaName);
	~ArenaApp();

	bool InitializeControls() override;
	TString GetArenaName();

	RECT Get3DArea()override;

	void SetColor(D2D1_COLOR_F c);

	void HandleEvents(TDataArray<EventID_Cred>& cred) override;
	TDataArray<ArenaBuilderEvents> arenaHandlers;
	void SetArenaName(TString& name);

	void UpdatePanelText();

	void UpdateCameraText();
	void UpdateModelText();
protected:
	void UpdatePosDirText(DirectX::XMFLOAT3& d, DirectX::XMFLOAT3& l);

	TArena* arena;
	TrecPointer<TArenaEngine> modelCollection;

	TTrecPointerArray<ArenaModel> basicModels;
	TrecPointer<ArenaModel> currentModel;
	bool focusOnModel;

	TrecPointer<ArenaModel> scale;
	TTextField *d_x, *d_y, *d_z, *l_x, *l_y, *l_z;
	TrecPointer<TControl> camModToggleSign;
	bool rotateMode;
	TString arenaName;


	void TextDirectionX(TrecPointer<TControl> tc, EventArgs ea);
	void TextLocationX(TrecPointer<TControl> tc, EventArgs ea);
	void TextDirectionY(TrecPointer<TControl> tc, EventArgs ea);
	void TextLocationY(TrecPointer<TControl> tc, EventArgs ea);
	void TextDirectionZ(TrecPointer<TControl> tc, EventArgs ea);
	void TextLocationZ(TrecPointer<TControl> tc, EventArgs ea);
	void OnUp(TrecPointer<TControl> tc, EventArgs ea);
	void OnDown(TrecPointer<TControl> tc, EventArgs ea);
	void OnLeft(TrecPointer<TControl> tc, EventArgs ea);
	void OnRight(TrecPointer<TControl> tc, EventArgs ea);
	void OnNear(TrecPointer<TControl> tc, EventArgs ea);
	void OnFar(TrecPointer<TControl> tc, EventArgs ea);
	void OnSetCameraRotate(TrecPointer<TControl> tc, EventArgs ea);
	void OnSetCameraTranslate(TrecPointer<TControl> tc, EventArgs ea);

	void OnSelectObject(TrecPointer<TControl> tc, EventArgs ea);
	void OnToggleObjectAndCamera(TrecPointer<TControl> tc, EventArgs ea);

	void OnGetDefaultObject(TrecPointer<TControl> tc, EventArgs ea);
};

