#include "AnimationBuilder.h"
#include "ColorAnimation.h"
#include "RotateAnimation.h"
#include "TransitionAnimation.h"
#include "GifAnimation.h"
#include "TControl.h"

/*
* Method: AnimationBuilder::AnimationBuilder
* Purpose: Constructor
* Parameters: void
* Returns: New Animation Builder
*/
AnimationBuilder::AnimationBuilder()
{
	phase = animation_phase::animation_phase_middle;
}

/*
* Method: AnimationBuilder::SetName
* Purpose: Sets the name to give any animation created by this builder
* Parameters: const TString& name - the name to set the animation to
* Returns: void
*/
void AnimationBuilder::SetName(const TString& name)
{
	this->name.Set(name);
}

/*
* Method: AnimationBuilder::SetType
* Purpose: Sets the type of Animation
* Parameters: const TString& type - the type to set
* Returns: void
*/
void AnimationBuilder::SetType(const TString& type)
{
	this->type.Set(type);
}

/*
* Method: AnimationBuilder::SetAttribute
* Purpose: Adds specifications for the animation to be created
* Parameters: const TString& key - the animation attribute to add
*				TrecPointer<TString> value - the value of the anumation attribute
* Returns: void
*/
void AnimationBuilder::SetAttribute(const TString& key, TrecPointer<TString> value)
{
	if (!key.Compare(L"Phase"))
	{
		if (value.Get())
		{
			if (!value->Compare(L"Begin"))
				phase = animation_phase::animation_pase_beginning;
			else if (!value->Compare(L"End"))
				phase = animation_phase::animation_phase_end;
			else if (!value->Compare(L"Persistent"))
				phase = animation_phase::animation_phase_persistant;
		}
		return;
	}
	if (value.Get())
		attributes.addEntry(key, value);
}

/*
* Method: AnimationBuilder::GetName
* Purpose: Reports the name of the animation to create
* Parameters: void
* Returns: TString - the name of the animation to create
*/
TString AnimationBuilder::GetName()
{
	return name;
}

/*
* Method: AnimationBuilder::GetType
* Purpose: Reports the type of animation to create
* Parameters: void
* Returns: TString - the type of animation given
*/
TString AnimationBuilder::GetType()
{
	return type;
}

/*
* Method: AnimationBuilder::Build
* Purpose: Builds the animation
* Parameters: void
* Returns: TrecPointer<Animation> - the Animation requested
*/
TrecPointer<Animation> AnimationBuilder::Build()
{
	TrecPointer<Animation> ret;

	if (!type.CompareNoCase(TString(L"Simple-Color")))
		ret = TrecPointerKey::GetNewTrecPointerAlt<Animation, ColorAnimation>(phase);
	else if (!type.CompareNoCase(TString(L"Simple-Transition")))
		ret = TrecPointerKey::GetNewTrecPointerAlt<Animation, TransitionAnimation>(phase);
	else if (!type.CompareNoCase(TString(L"Simple-Rotation")))
		ret = TrecPointerKey::GetNewTrecPointerAlt<Animation, RotateAnimation>(phase);
	else if (!type.CompareNoCase(TString(L"Gif")))
		ret = TrecPointerKey::GetNewTrecPointerAlt<Animation, GifAnimation>(phase);


	if (!ret.Get())
		return ret;

	UINT index = 0;

	TrecPointer<tEntry<TString>> entry = attributes.GetEntryAt(index++);

	while (entry.Get() && entry->object.Get())
	{
		D2D1_COLOR_F color;
		ColorFormat cFormat;
		float value = 0.0f;
		if (!entry->key.Compare(L"|BeginColor") && entry->object->ConvertToColor(color, cFormat))
		{
			ret->SetAnimationValue(color.a, animation_value_type::animation_value_type_alpha);
			ret->SetAnimationValue(color.b, animation_value_type::animation_value_type_blue);
			ret->SetAnimationValue(color.g, animation_value_type::animation_value_type_green);
			ret->SetAnimationValue(color.r, animation_value_type::animation_value_type_red);
		}
		else if (!entry->key.Compare(L"|EndColor") && entry->object->ConvertToColor(color, cFormat))
		{
			ret->SetAnimationValue(color.a, animation_value_type::animation_value_type_alpha2);
			ret->SetAnimationValue(color.b, animation_value_type::animation_value_type_blue2);
			ret->SetAnimationValue(color.g, animation_value_type::animation_value_type_green2);
			ret->SetAnimationValue(color.r, animation_value_type::animation_value_type_red2);
		}
		else if (!entry->key.Compare(L"|IsRadian"))
		{
			// Value could be anything, all this does is set animation to radian mode (default is degrees)
			ret->SetAnimationValue(0.0f, animation_value_type::animation_value_type_radian);
		}
		else if (!entry->key.Compare(L"|BeginRotate") && !entry->object->ConvertToFloat(value))
		{
			ret->SetAnimationValue(value, animation_value_type::animation_value_type_rotate_start);
		}
		else if (!entry->key.Compare(L"|EndRotate") && !entry->object->ConvertToFloat(value))
		{
			ret->SetAnimationValue(value, animation_value_type::animation_value_type_rotate_end);
		}
		else if (!entry->key.Compare(L"|RotateDirection"))
		{
			entry->object->ConvertToFloat(value);
			ret->SetAnimationValue(value, animation_value_type::animation_value_type_direction);
		}
		else if (!entry->key.Compare(L"|Location"))
		{
			auto rect = convertStringToD2D1Rect(entry->object.Get());
			ret->SetAnimationValue(rect.bottom, animation_value_type::animation_value_type_bottom_location);
			ret->SetAnimationValue(rect.left, animation_value_type::animation_value_type_left_location);
			ret->SetAnimationValue(rect.top, animation_value_type::animation_value_type_top_location);
			ret->SetAnimationValue(rect.right, animation_value_type::animation_value_type_right_location);

		}
		else if (!entry->key.Compare(L"|Length"))
		{
			int length = 0;
			if (!entry->object->ConvertToInt(length))
				ret->SetMillisecondLength(length);
		}
		else if (!entry->key.Compare(L"|RefreshRate"))
		{
			int length = 0;
			if (!entry->object->ConvertToInt(length))
				ret->SetMillisecondRefresh(length);
		}








		entry = attributes.GetEntryAt(index++);
	}


	return ret;
}
