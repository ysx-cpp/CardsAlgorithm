#include "mahjongalgo.h"
#include "GeneratorFiles/rpc_MjEnums.h"

namespace mahjongalgorithm {

void MjHuPaiAnalyzer::initContext(const CTargetPtr target, ISeatPtr seat, int funCardNum)
{
	m_targetPtr_ = target;
	m_seatPtr_ = seat;
	m_huPaiAlgo_.SetLaiZiNum(seat->GetLaiZiCnt());
	uint8_t c = m_huPaiAlgo_.recoverCard(seat->GetLaiZiCard()->typeId(), seat->GetLaiZiCard()->faceId());
	m_huPaiAlgo_.SetLaiCard(c);
	m_huPaiAlgo_.InputHandCard(seat->GetHandCard());
}

bool MjHuPaiAnalyzer::doTestHuCard()
{
	bool bIsHu = false;
	CHuPaiResult huResult;

	if (m_huPaiAlgo_.CheckQiDuiHu())
	{
		bIsHu = true;
	}
	else if (m_huPaiAlgo_.CheckShiSanYaoHu())
	{
		 bIsHu = true;
	}
	else if (m_huPaiAlgo_.CheckPingHu())
	{
		bIsHu = true;
	}

	if (bIsHu)
	{
		m_huPaiAlgo_.OutPutDoorCards(huResult.m_vecDoorCardGrp_);
		for (auto &iter : huResult.m_vecDoorCardGrp_)
		{
			iter->m_nOtherSeat = -1;
			if (!m_targetPtr_->m_bChoose_ && m_targetPtr_->m_cardPtr_)
			{
				auto it = find(iter->m_vSelfCard.begin(), iter->m_vSelfCard.end(), m_targetPtr_->m_cardPtr_->indexId());
				if (it != iter->m_vSelfCard.end())
				{
					iter->m_nOtherCard = *it;
					iter->m_nOtherSeat = m_targetPtr_->m_nSeat_;
					iter->m_nHuCard = m_targetPtr_->m_nSeat_;
					iter->m_vSelfCard.erase(it);
					break;
				}
			}
		}
		m_mapSeat2HupaiResult_[m_seatPtr_->Id()] = huResult;
		return true;
	}

	return false;
}

bool MjHuPaiAnalyzer::calcEatCardGroup()
{
	return false;
}

bool MjHuPaiAnalyzer::calcMingTang(std::int32_t mingtang_flag)
{
	return false;
}

CHuPaiResult* MjHuPaiAnalyzer::getHuPaiResult(int32_t seat_no)
{
	if (m_mapSeat2HupaiResult_.find(seat_no) != m_mapSeat2HupaiResult_.end())
		return &m_mapSeat2HupaiResult_[seat_no];
	return nullptr;
}

vector<vector<DoorGroupPtr>>& MjHuPaiAnalyzer::getChiPaiResult(int32_t seat_no)
{
	return m_chiResult_;
};

} // namespace mahjongalgorithm