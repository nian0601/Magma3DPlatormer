#include "stdafx.h"

#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "RenderProcessor.h"

#include <PostMaster.h>
#include <RenderMessage.h>

RenderProcessor::RenderProcessor(Magma::World& aWorld)
	: Magma::BaseProcessor(aWorld, Magma::CreateFilter<Magma::Requires<TranslationComponent, RenderComponent>>())
{
}


RenderProcessor::~RenderProcessor()
{
}

void RenderProcessor::Update(float aDelta)
{
	CU::GrowingArray<Magma::Entity> entities = GetEntities();
	for each (const Magma::Entity& entity in entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		RenderComponent& render = GetComponent<RenderComponent>(entity);

		PostMaster::GetInstance()->SendMessage(RenderMessage(translation.myOrientation, translation.myScale, render.myModelID, render.myEffectID));
	}
}
