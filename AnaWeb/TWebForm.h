#pragma once
#include "TWebNode.h"


/**
 * Enum Class: web_input_type
 * Purpose: Alows input Nodes to track which input type they are
 */
typedef enum class web_input_type
{
    wit_button,         // Button input
    wit_checkbox,       // Checkbox input
    wit_color,          // Color input
    wit_date,           // Date input
    wit_datetime_local, // date-time-local
    wit_email,          // email
    wit_file,           // file
    wit_hidden,         // Hidden, holds data but not presented to the user
    wit_image,          // scans for images
    wit_month,          // months
    wit_number,         // numbers
    wit_password,       // password
    wit_radio,          // radio button
    wit_range,          // Range
    wit_reset,          // Resets all values to initial points
    wit_search,         // Search
    wit_submit,         // Submit button
    wit_tel,            // Not sure
    wit_text,           // Basic text (default)
    wit_time,           // Time
    wit_url,            // Expects a URL
    wit_week
}web_input_type;

/**
 * Class: TWebForm
 * Purpose: Handles form specific functionality on top of being a WebNode
 * SuperClass: TWebNode - a web form is a web/HTML node
 */
class TWebForm :
    public TWebNode
{
public:

    /**
     * Method: TWebForm::TWebForm
     * Purpose: Constructor
     * Parameters: TrecPointer<DrawingBoard> board - the board to draw on
     * Returns: new Html Node that acts like a form
     */
    TWebForm(TrecPointer<DrawingBoard> board);


    /**
     * Method: TWebForm::CreateWebNode
     * Purpose: Sets up the Web Node for Rendering, same purpose as TControl::onCreate()
     * Parameters: D2D1_RECT_F location - the location within the Window the Node is expected to operate in
     *              TrecPointer<TWindowEngine> d3dEngine - Pointer to the 3D manager, for controls with a 3D component to them
     *              HWND window - handle to the window the node is operating in
     *              TrecPointerSoft<TWebNode> parent - the node that called this method
     * Returns: UINT - Error Code
     */
    virtual UINT CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window) override;

protected:


    /**
     * Method: TWebForm::CompileProperties_
     * Purpose: Finalize propertie form the generated lists (virtual so that special nodes can look for special attributes)
     * Parameters: const TrecPointer<TArray<styleTable>>& atts - list of CSS
     * Returns: void
     */
    virtual void CompileProperties(TDataMap<TString>& atts)override;

    /**
     * Whether a GET or Post should be performed
     */
    bool doGet;

    /**
     * The action to take
     */
    TString action;

    /**
     * Action to take upon submission
     */
    TString onSubmit;
};

/**
 * Class: TWebInput
 * Purpose: Handles Specific input types
 * 
 * SuperClass: TWebNode
 */
class TWebInput : public TWebNode
{

    /**
     * Method: TWebInput::TWebInput
     * Purpose: Constructor
     * Parameters: TrecPointer<DrawingBoard> board - the board to draw on
     * Returns: new Html Node that handles input
     */
    TWebInput(TrecPointer<DrawingBoard> board);


    /**
     * Method: TWebInput::CreateWebNode
     * Purpose: Sets up the Web Node for Rendering, same purpose as TControl::onCreate()
     * Parameters: D2D1_RECT_F location - the location within the Window the Node is expected to operate in
     *              TrecPointer<TWindowEngine> d3dEngine - Pointer to the 3D manager, for controls with a 3D component to them
     *              HWND window - handle to the window the node is operating in
     *              TrecPointerSoft<TWebNode> parent - the node that called this method
     * Returns: UINT - Error Code
     */
    virtual UINT CreateWebNode(D2D1_RECT_F location, TrecPointer<TWindowEngine> d3dEngine, HWND window) override;

protected:

    /**
     * Method: TWebInput::CompileProperties_
     * Purpose: Finalize propertie form the generated lists (virtual so that special nodes can look for special attributes)
     * Parameters: const TrecPointer<TArray<styleTable>>& atts - list of CSS
     * Returns: void
     */
    virtual void CompileProperties(TDataMap<TString>& atts)override;

    /**
     * Input type
     */
    web_input_type inputType;
};


