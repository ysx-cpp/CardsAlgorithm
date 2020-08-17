#include "CTarget.h"
#include "ICard.h"
namespace by
{
    CTarget::CTarget()
    {

    }

    string CTarget::LogString()
    {
        return boost::str(boost::format("Target detail: provider:%1%,choose:%2%,card:%3%") % m_nSeat_ % m_bChoose_ % (NULL != m_cardPtr_.get() ? m_cardPtr_->Name() : "-"));
    }
}