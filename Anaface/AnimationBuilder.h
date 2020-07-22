#pragma once
#include <TObject.h>
#include <TrecReference.h>
#include <Animation.h>
#include <TMap.h>
#include "Anaface.h"

/**
 * Class: AnimationBuilder
 * Purpose: Holds basic information and attributes about an animation template until it can be used to create an animation object
 */
class _ANAFACE_DLL AnimationBuilder : public TObject
{
public:

	/*
	* Method: AnimationBuilder::AnimationBuilder
	* Purpose: Constructor
	* Parameters: void
	* Returns: New Animation Builder
	*/
	AnimationBuilder();
	/*
	* Method: AnimationBuilder::SetName
	* Purpose: Sets the name to give any animation created by this builder
	* Parameters: const TString& name - the name to set the animation to
	* Returns: void
	*/
	void SetName(const TString& name);
	/*
	* Method: AnimationBuilder::SetType
	* Purpose: Sets the type of Animation
	* Parameters: const TString& type - the type to set
	* Returns: void
	*/
	void SetType(const TString& type);
	/*
	* Method: AnimationBuilder::SetAttribute
	* Purpose: Adds specifications for the animation to be created
	* Parameters: const TString& key - the animation attribute to add
	*				TrecPointer<TString> value - the value of the anumation attribute
	* Returns: void
	*/
	void SetAttribute(const TString& key, TrecPointer<TString> value);

	/*
	* Method: AnimationBuilder::GetName
	* Purpose: Reports the name of the animation to create
	* Parameters: void
	* Returns: TString - the name of the animation to create
	*/
	TString GetName();
	/*
	* Method: AnimationBuilder::GetType
	* Purpose: Reports the type of animation to create
	* Parameters: void
	* Returns: TString - the type of animation given
	*/
	TString GetType();

	/*
	* Method: AnimationBuilder::Build
	* Purpose: Builds the animation
	* Parameters: void
	* Returns: TrecPointer<Animation> - the Animation requested
	*/
	TrecPointer<Animation> Build();
protected:
	/*
	* The name and Type of Animation to create
	*/
	TString name, type;
	/*
	* List of attributes to give the animation once created
	*/
	TMap<TString> attributes;
	/*
	* When this animation is to run
	*/
	animation_phase phase;
};

