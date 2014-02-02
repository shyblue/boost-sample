// multi_index_test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <WinSock2.h>
#include <iostream>
#include <time.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/identity.hpp>

#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <tbb/concurrent_queue.h>

typedef struct _tPlayerCard
{
	int card_no;
	int card_team;
	int card_year;
	int card_pos;
	int card_gold;
	std::string card_grade;
	std::string card_player_name;
} TCardInfo;

struct key_CardNo{};
struct key_TeamYearGold{};
struct key_YearPosGold{};

using namespace boost::multi_index;

typedef boost::multi_index_container<
	TCardInfo, 
	indexed_by
	<
	    ordered_unique< tag<key_CardNo>, BOOST_MULTI_INDEX_MEMBER(TCardInfo,int,card_no ) > ,
		ordered_non_unique< tag<key_TeamYearGold>, composite_key< TCardInfo, BOOST_MULTI_INDEX_MEMBER(TCardInfo,int, card_team), BOOST_MULTI_INDEX_MEMBER(TCardInfo,int, card_year), BOOST_MULTI_INDEX_MEMBER(TCardInfo, int, card_gold) > >
	>
> cardContainer;


int _tmain(int argc, _TCHAR* argv[])
{
	boost::variate_generator<boost::mt19937, boost::uniform_int<> > rand( boost::mt19937(static_cast<boost::uint32_t>(time(0))), boost::uniform_int<>(1,10) );

	cardContainer myCards;

	for(int i=0; i<100; i++)
	{

		TCardInfo temp;
		temp.card_no=i;
		temp.card_team = i%10;
		temp.card_year = 1999;
		temp.card_gold = rand();
		temp.card_pos = rand();
		temp.card_grade = "N";
		temp.card_player_name = "Player" + i;

		myCards.insert(temp);
	}
	
	const auto& myTag = myCards.get<key_CardNo>();
	for( auto iter = myTag.begin(); iter != myTag.end(); ++iter)
	{
		const TCardInfo &temp = (*iter);

		std::cout << "Card No : " << temp.card_no << " | Team : " << temp.card_team << " | Gold : " << temp.card_gold << " | Pos : " << temp.card_pos << std::endl;
	}

	std::cout << "==============================================" << std::endl;
	const auto& idx2 = myCards.get<key_TeamYearGold>();
	auto itor = idx2.equal_range(boost::make_tuple(1));

	while(itor.first != itor.second)
	{
		const TCardInfo &temp = (*(itor.first));

		std::cout << "Card No : " << temp.card_no << " | Team : " << temp.card_team << " | Gold : " << temp.card_gold << " | Pos : " << temp.card_pos << std::endl;

		++itor.first;
	}

	std::cout << "==============================================" << std::endl;
	itor.first = idx2.upper_bound(boost::make_tuple(1,1999,2));
	itor.second = idx2.lower_bound(boost::make_tuple(1,1999,8));
	while(itor.first != itor.second)
	{
		const TCardInfo &temp = (*(itor.first));

		std::cout << "Card No : " << temp.card_no << " | Team : " << temp.card_team << " | Gold : " << temp.card_gold << " | Pos : " << temp.card_pos << std::endl;

		++itor.first;
	}
	

	return 0;
}

