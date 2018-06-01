#include "ScoreCounter.hpp"
#include <SimpleEngineApp.hpp>
#include <Resource/ResourceManager.hpp>
#include <Graphics2D/Texture.hpp>
#include <Graphics2D/Renderer.hpp>


const EventType EventDataUpdateScore::eventType = 0x789b4779;


/*
==================
ScoreCounter::ScoreCounter
==================
*/
ScoreCounter::ScoreCounter(const int x, const int y) : m_x(x), m_y(y), m_score(0), m_digitsTexture(g_app->GetResourceMan()->GetHandle("textures\\digits.png")), m_visible(true) {
	if (m_digitsTexture) {
		g_app->GetEventManager()->AddListener(EventListenerDelegate::create<ScoreCounter, &ScoreCounter::ScoreUpdateDelegate>(this), EventDataUpdateScore::eventType);
		auto textureExtraData = std::static_pointer_cast<TextureExtraData>(m_digitsTexture->GetExtra());

		m_digitWidth = textureExtraData->GetWidth() / DIGITS_COUNT;
	}
}

/*
==================
ScoreCounter::~ScoreCounter
==================
*/
ScoreCounter::~ScoreCounter() {
	g_app->GetEventManager()->RemoveListener(EventListenerDelegate::create<ScoreCounter, &ScoreCounter::ScoreUpdateDelegate>(this), EventDataUpdateScore::eventType);
}

/*
==================
ScoreCounter::OnRender
==================
*/
void ScoreCounter::OnRender(const float deltaTimeMs) {
	if (m_visible && m_digitsTexture) {
		auto texture = std::static_pointer_cast<TextureExtraData>(m_digitsTexture->GetExtra());
		g_app->GetRenderer()->EnableAlphaBlending(texture);
		auto renderer = g_app->GetRenderer();

		int tmpX = m_x + m_digitWidth * MAX_DIGITS_COUNT;

		for (int i = 0; i < MAX_DIGITS_COUNT; i++, tmpX -= m_digitWidth) {
			if (m_frameNums[i] != -1) {
				renderer->RenderSprite(texture, tmpX, m_y, DIGITS_COUNT, m_frameNums[i]);
			}
		}
	}
}

/*
==================
ScoreCounter::ScoreUpdateDelegate
==================
*/
void ScoreCounter::ScoreUpdateDelegate(IEventPtr eventData) {
	auto castEventData = std::static_pointer_cast<EventDataUpdateScore>(eventData);

	m_score = castEventData->GetScore();
	int tmpScore = m_score;

	m_frameNums[MAX_DIGITS_COUNT - 1] = tmpScore % 10;		// we must draw at least one zero
	tmpScore /= 10;

	for (int i = MAX_DIGITS_COUNT - 1 - 1; i > 0; i--) {
		if (tmpScore) {
			m_frameNums[i] = tmpScore % 10;
			tmpScore /= 10;
		} else {
			m_frameNums[i] = -1;		//mark digit as void
		}
	}
}
