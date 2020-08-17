#ifndef MJGAME_MAHJONG_ALGO_H
#define MJGAME_MAHJONG_ALGO_H

#include "mahjongalgo.h"
#include "gameBase/calc/CHuPaiAnalyzer.h"

namespace mahjongalgorithm {

class MjHuPaiAnalyzer : public IHuPaiAnalyzer
{
public:
	virtual void init(IGameCalcPtr	game_calc_ptr, ICardGeneratorPtr cardGeneratorPtr, int funCardNum, int minHuXi, int allowPairCnt = 0)
	{
	}

	virtual void initContext(const CTargetPtr target, ISeatPtr seat, int funCardNum);
	virtual bool doTestHuCard();

	virtual bool calcEatCardGroup();

	virtual bool calcMingTang(std::int32_t mingtang_flag);
	virtual CHuPaiResult* getHuPaiResult(int32_t seat_no);

	virtual vector<vector<DoorGroupPtr>>& getChiPaiResult(int32_t seat_no);

private:
	CTargetPtr    m_targetPtr_;
	ISeatPtr   m_seatPtr_;
	vector<vector<DoorGroupPtr>> m_chiResult_;
	map<int32_t, CHuPaiResult> m_mapSeat2HupaiResult_;
	mahjongalgorithm::HuPaiAlgorithm m_huPaiAlgo_;
};

} // namespace mahjongalgorithm
#endif // MJGAME_MAHJONG_ALGO_H