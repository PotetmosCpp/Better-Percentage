#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
	struct Fields {
		float m_runFrom;
		std::unordered_map<double, double> m_bestRunEnds;	// dumb name i think idk
	};

	void destroyPlayer(PlayerObject* player, GameObject* object) {
		PlayLayer::destroyPlayer(player, object);

		if (m_isPlatformer) return;

		if (object != m_anticheatSpike) {
			if (PlayLayer::getCurrentPercent() > m_fields->m_bestRunEnds[m_fields->m_runFrom]) {
				m_fields->m_bestRunEnds[m_fields->m_runFrom] = PlayLayer::getCurrentPercent();
			}
		}
	}

	void resetLevel() {
		PlayLayer::resetLevel();

		if (m_isPlatformer) return;

		m_fields->m_runFrom = PlayLayer::getCurrentPercent();
	}

	void levelComplete() {
		PlayLayer::levelComplete();

		if (m_isPlatformer) return;

		m_fields->m_bestRunEnds[m_fields->m_runFrom] = 100;
	}

	void updateProgressbar() {
		PlayLayer::updateProgressbar();

		if (m_isPlatformer) return;

		auto decimals = Mod::get()->getSettingValue<int>("decimals");
		auto showPercentSymbol = Mod::get()->getSettingValue<bool>("show-percent-symbol");

		auto showRunFrom = Mod::get()->getSettingValue<bool>("show-run-from");
		auto hideRunFromFromZero = Mod::get()->getSettingValue<bool>("hide-run-from-from-zero");

		auto showBestRun = Mod::get()->getSettingValue<bool>("show-best-run");
		auto hideBestRunFromZero = Mod::get()->getSettingValue<bool>("hide-best-run-from-zero");
		auto showLevelBestFromZero = Mod::get()->getSettingValue<bool>("show-level-best-from-zero");

		auto seperatorsWithSpacing = Mod::get()->getSettingValue<bool>("seperators-with-spacing");

		auto formatNumber = [decimals](double value) {
			return geode::utils::numToString(value, decimals);	// probably does the same idk
			//return fmt::format("{:.{}f}", value, decimals);
		};

		auto addSpaces = [seperatorsWithSpacing](std::string string) {
			if (seperatorsWithSpacing) {
				return " " + string + " ";
			} else {
				return string;
			}
		};

		auto runFrom = (showRunFrom)
			? ((m_fields->m_runFrom == 0 && hideRunFromFromZero)
				? ""
				: formatNumber(m_fields->m_runFrom) + addSpaces("-")
			)
			: "";

		auto bestRun = (showBestRun && !(m_fields->m_runFrom == 0 && hideBestRunFromZero))
			? ((m_fields->m_runFrom == 0 && showLevelBestFromZero)
				? addSpaces("/") + formatNumber(m_level->m_normalPercent.value())
				: ((m_fields->m_bestRunEnds.count(m_fields->m_runFrom))
			 		? addSpaces("/") + formatNumber(m_fields->m_bestRunEnds[m_fields->m_runFrom])	// wtf is this
					: ""
				)
	  		)
			: "";

		auto percent = (showPercentSymbol)
			? "%"
			: "";

		m_percentageLabel->setString((
			runFrom +
			formatNumber(PlayLayer::getCurrentPercent()) +
			bestRun +
			percent
		).c_str());
	}
};
