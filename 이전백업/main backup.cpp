/*
ⓒ 2017. Lee Seung-won, Kim JIn-woo, Mun Kyeong-jin All rights reserved.
*/

#include "modoo_oop_2.h" //2로 수정

int main()
{
	vector<Land*> l(MAXLAND);
	vector<Person*> p(3);

	try {
		loadData(l, p);
	}
	catch (...) {
		cout << "데이터를 정상적으로 불러올 수 없습니다. 게임을 종료합니다." << endl;
		return 0;
	}

	cout << endl;
	cout << "성대의 마블 v1.0" << endl;
	cout << endl;
	cout << "주사위 던질 순서를 정할게요." << endl;
	cout << endl;

	int order[3];

	srand(int(time(NULL)));
	int count = 0;
	while (count < 3)
	{
		int is_checked = 0;
		int num = rand() % 3 + 1;
		for (int i = 0; i < 3; i++)
		{
			if (num == order[i])
			{
				is_checked = 1;
			}
		}
		if (is_checked == 0)
		{
			order[count] = num;
			count++;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		p[i] = new Person(order[i], "");
	}
	cout << "첫번째 차례: " << p[0] << endl;
	cout << "두번째 차례: " << p[1] << endl;
	cout << "세번째 차례: " << p[2] << endl;
	cout << endl;

	bool is_double;
	int turn = 1;

	//독점 미적용
	while (p.size() > 1 && turn <= 20)
	{
		for (int i = 0; i < (int)p.size(); i++)
		{
			int double_count = 0;
			do
			{
				display(p, l);
				is_double = false;
				int key;
				cout << p[i] << "님의 턴이에요." << endl;

				if (p[i]->retPenalty() > 0)
				{
					//freejail 반환 함수 없음
					cout << "현재 학사경고 칸에 있어요. 지금부터 " << p[i]->retPenalty() << "턴 동안은 움직일 수 없어요!" << endl;
					cout << "주사위를 던져 더블이 나오면 탈출할 수 있어요." << endl;
					cout << "주사위를 던지려면 3을 입력하세요." << endl;
					key = getNumber(3, 3);
					cout << "주사위를 던지는 중이에요." << endl;
					int dice_value = p[i]->role(is_double, key);
					cout << dice_value << "! ";
					if (is_double)
					{
						cout << "더블!" << endl;
						cout << "성공!" << endl;
					}
					else
					{
						cout << "실패!" << endl;
						break;
					}
				}
				
				if (p[i]->retOdd() > 0 && p[i]->retEven() > 0)
				{
					cout << "현재 홀수 카드를 " << p[i]->retOdd() << "장, 짝수 카드를 " << p[i]->retEven() << "장 가지고 있어요." << endl;
					cout << "홀수 카드를 사용하려면 1, 짝수 카드를 사용하려면 2, 카드를 사용하지 않으려면 3을 입력하세요." << endl;
					key = getNumber(1, 3);
					if (key == 1)
					{
						p[i]->useDiceItem(1);
					}
					else if (key == 2)
					{
						p[i]->useDiceItem(2);
					}
				}
				else if (p[i]->retOdd() > 0)
				{
					cout << "현재 홀수 카드를 " << p[i]->retOdd() << "장 가지고 있어요." << endl;
					cout << "홀수 카드를 사용하려면 1, 카드를 사용하지 않으려면 3을 입력하세요." << endl;
					int arr[2] = { 1, 3 };
					key = getNumber(arr, 2);		
					if (key == 1)
					{
						p[i]->useDiceItem(1);
					}
				}
				else if (p[i]->retEven() > 0)
				{
					cout << "현재 짝수 카드를 " << p[i]->retEven() << "장 가지고 있어요." << endl;
					cout << "짝수 카드를 사용하려면 2, 카드를 사용하지 않으려면 3을 입력하세요." << endl;
					key = getNumber(2, 3);
					if (key == 2)
					{
						p[i]->useDiceItem(2);
					}
				}
				else
				{
					cout << "주사위를 던지려면 3을 입력하세요." << endl;
					key = getNumber(3, 3);
				}

				cout << "주사위를 던지는 중이에요." << endl;
				int dice_value = p[i]->role(is_double, key);
				cout << dice_value << "! ";
				if (is_double)
				{
					cout << "더블!";
					double_count++;
				}

				cout << endl;

				if (double_count == 3)
				{
					p[i]->gotoJail();
					
					//freejail 관련 함수 없음
					cout << "학사경고 칸에 도착했어요. 지금부터 3턴 동안은 움직일 수 없어요!" << endl;

					break;
				}

				p[i]->move(dice_value);

				bool flag = true;
				while (flag) {
					flag = false;
					if (isHiddenCard(p[i]->myPosition())) {
						cout << "히든카드를 뽑도록 해봅시다" << endl;
						flag = p[i]->makeHiddenCard(&l);
					}
					if (p[i]->myPosition() == GO_BACK)
					{
						display(p, l);
						cout << "거꾸로 갑니다~ 꽉 잡으세요!" << endl;
						cout << "주사위를 던지려면 3을 입력하세요." << endl;
						key = getNumber(3, 3);
						cout << "주사위를 던지는 중이에요." << endl;
						dice_value = p[i]->role(is_double, key);
						cout << dice_value << "만큼 뒤로 이동합니다. " << endl;
						p[i]->move(dice_value *= -1);
						display(p, l);
						flag = true;
					}
					if (p[i]->myPosition() == SPACE_SHIP) {
						cout << "이동하고 싶은 곳의 번호를 입력해주세요." << endl;
						key = getNumber(0, 19);
						p[i]->moveForced(key);
						flag = true;
					}

				}

				if (p[i]->myPosition() == START)
				{
					p[i]->calMoney(SALARY);
					bool is_owned = false;
					for (int j = 0; j < 19; j++)
					{
						if (l[j]->owner() == p[i])
						{
							is_owned = true;
							break;
						}
					}
					if (is_owned)
					{
						cout << "추가 건설을 원하는 지역의 번호를 입력하세요." << endl;
						while (true)
						{
							key = getNumber(0, 19);
							if (l[key]->owner() == p[i])
							{
								break;
							}
							else
							{
								cout << "주인이 아닌 땅을 선택했어요. 다시 입력하세요." << endl;
							}
						}

						if (l[p[i]->myPosition()]->numOfAmenties() == 3)
						{
							cout << "랜드마크를 건설할까요? 랜드마크를 건설하려면 1, 건설하지 않으려면 2를 입력하세요. " << endl;
							cout << "랜드마크 건설비용은 " << l[p[i]->myPosition()]->costBuy(3) << "이에요." << endl;
							key = getNumber(1, 2);
							if (key == 1)
							{
								cout << "랜드마크 건설!" << endl;
								p[i]->buyLandMark(l[p[i]->myPosition()], key);
							}
						}
						else
						{
							if (l[p[i]->myPosition()]->numOfAmenties() == 0)
							{
								cout << "부대시설을 구입할까요? 구입할 부대시설의 수(1~3)를 입력하세요. 구입하지 않으려면 0을 입력하세요." << endl;
								cout << "현재 소유권만 가지고 있어요." << endl;
								cout << endl;
								cout << "부대시설 1개: " << l[p[i]->myPosition()]->costBuy(1) << endl;
								cout << "부대시설 2개: " << l[p[i]->myPosition()]->costBuy(1) + l[p[i]->myPosition()]->costBuy(2) << endl;
								cout << "부대시설 3개: " << l[p[i]->myPosition()]->costBuy(1) + l[p[i]->myPosition()]->costBuy(2) + l[p[i]->myPosition()]->costBuy(3) << endl;
							}
							else if (l[p[i]->myPosition()]->numOfAmenties() == 1)
							{
								cout << "부대시설을 구입할까요? 구입할 부대시설의 수(1~2)를 입력하세요. 구입하지 않으려면 0을 입력하세요." << endl;
								cout << "현재 부대시설을 1개 가지고 있어요." << endl;
								cout << endl;
								cout << "부대시설 1개: " << l[p[i]->myPosition()]->costBuy(2) << endl;
								cout << "부대시설 2개: " << l[p[i]->myPosition()]->costBuy(2) + l[p[i]->myPosition()]->costBuy(3) << endl;
							}
							else if (l[p[i]->myPosition()]->numOfAmenties() == 2)
							{
								cout << "부대시설을 구입할까요? 구입할 부대시설의 수(1)를 입력하세요. 구입하지 않으려면 0을 입력하세요." << endl;
								cout << "현재 부대시설을 2개 가지고 있어요." << endl;
								cout << endl;
								cout << "부대시설 1개: " << l[p[i]->myPosition()]->costBuy(3) << endl;
							}
							key = getNumber(0, 3);
							if (key >= 1)
							{
								//buyLand 수정 필요: 소유권이 있는 지역에서 부대시설 구입시 입력값 -1에서 0으로 변경
								p[i]->buyLand(l[p[i]->myPosition()], key);
							}
						}
					}
				}
				if (p[i]->myPosition() == JAIL)
				{
					cout << "Jail 칸에 도착했어요. 지금부터 3턴 동안은 움직일 수 없어요!" << endl;
				}

				if (p[i]->myPosition() == GAMBLE)
				{
					cout << "Gamble 칸에 도착했어요. 미니게임을 하려면 1, 하지 않으려면 2를 입력하세요." << endl;
					cout << "미니게임 비용은 1000이에요." << endl;
					key = getNumber(1, 2);
					if (key == 1)
					{
						p[i]->Gamble(COST);
					}
				}

				if (isDorm(p[i]->myPosition()))
				{
					cout << l[p[i]->myPosition()] << "에 도착했어요." << endl;
					if (l[p[i]->myPosition()]->owner() == NULL)
					{
						cout << "기숙사를 구입할까요? 구입하려면 0, 구입하지 않으려면 -1을 입력하세요." << endl;
						key = getNumber(-1, 0);
						if (key == 0)
						{
							//기숙사 구입
						}
					}
					else
					{
						//통행료 지불
						//매각/파산
					}
				}

				if (l[p[i]->myPosition()]->costBuy(0) <= 0)
				{
					break;
				}

				cout << l[p[i]->myPosition()] << "에 도착했어요." << endl;

				if (!l[p[i]->myPosition()]->isLandmark())
				{
					if (l[p[i]->myPosition()]->owner() == NULL)
					{
						cout << "지역을 구입할까요? 구입할 부대시설의 수를 입력하세요(1~3). 지역만 구입하려면 0, 구입하지 않으려면 -1을 입력하세요." << endl;
						cout << endl;
						cout << "지역만: " << l[p[i]->myPosition()]->costBuy(0) << endl;
						cout << "부대시설 1개: " << l[p[i]->myPosition()]->costBuy(0) + l[p[i]->myPosition()]->costBuy(1) << endl;
						cout << "부대시설 2개: " << l[p[i]->myPosition()]->costBuy(0) + l[p[i]->myPosition()]->costBuy(1) + l[p[i]->myPosition()]->costBuy(2) << endl;
						cout << "부대시설 3개: " << l[p[i]->myPosition()]->costBuy(0) + l[p[i]->myPosition()]->costBuy(1) + l[p[i]->myPosition()]->costBuy(2) + l[p[i]->myPosition()]->costBuy(3) << endl;
						key = getNumber(-1, 3);
						if (key >= 0)
						{
							p[i]->buyLand(l[p[i]->myPosition()], key);
						}
					}
					else if (l[p[i]->myPosition()]->owner() == p[i])
					{
						if (l[p[i]->myPosition()]->numOfAmenties() == 3)
						{
							cout << "랜드마크를 건설할까요? 랜드마크를 건설하려면 1, 건설하지 않으려면 2를 입력하세요. " << endl;
							cout << "랜드마크 건설비용은 " << l[p[i]->myPosition()]->costBuy(3) << "이에요." << endl;
							key = getNumber(1, 2);
							if (key == 1)
							{
								cout << "랜드마크 건설!" << endl;
								p[i]->buyLandMark(l[p[i]->myPosition()], key);
							}
						}
						else
						{
							if (l[p[i]->myPosition()]->numOfAmenties() == 0)
							{
								cout << "부대시설을 구입할까요? 구입할 부대시설의 수(1~3)를 입력하세요. 구입하지 않으려면 0을 입력하세요." << endl;
								cout << "현재 소유권만 가지고 있어요." << endl;
								cout << endl;
								cout << "부대시설 1개: " << l[p[i]->myPosition()]->costBuy(1) << endl;
								cout << "부대시설 2개: " << l[p[i]->myPosition()]->costBuy(1) + l[p[i]->myPosition()]->costBuy(2) << endl;
								cout << "부대시설 3개: " << l[p[i]->myPosition()]->costBuy(1) + l[p[i]->myPosition()]->costBuy(2) + l[p[i]->myPosition()]->costBuy(3) << endl;
							}
							else if (l[p[i]->myPosition()]->numOfAmenties() == 1)
							{
								cout << "부대시설을 구입할까요? 구입할 부대시설의 수(1~2)를 입력하세요. 구입하지 않으려면 0을 입력하세요." << endl;
								cout << "현재 부대시설을 1개 가지고 있어요." << endl;
								cout << endl;
								cout << "부대시설 1개: " << l[p[i]->myPosition()]->costBuy(2) << endl;
								cout << "부대시설 2개: " << l[p[i]->myPosition()]->costBuy(2) + l[p[i]->myPosition()]->costBuy(3) << endl;
							}
							else if (l[p[i]->myPosition()]->numOfAmenties() == 2)
							{
								cout << "부대시설을 구입할까요? 구입할 부대시설의 수(1)를 입력하세요. 구입하지 않으려면 0을 입력하세요." << endl;
								cout << "현재 부대시설을 2개 가지고 있어요." << endl;
								cout << endl;
								cout << "부대시설 1개: " << l[p[i]->myPosition()]->costBuy(3) << endl;
							}
							key = getNumber(0, 3);
							if (key >= 1)
							{
								//buyLand 수정 필요: 소유권이 있는 지역에서 부대시설 구입시 입력값 -1에서 0으로 변경
								p[i]->buyLand(l[p[i]->myPosition()], key);
							}
						}
					}
					else
					{
						int fee = l[p[i]->myPosition()]->costFee(l[p[i]->myPosition()]->numOfAmenties());
						int take_over = l[p[i]->myPosition()]->retValue();//costBuy(l[p[i]->myPosition()]->numOfAmenties()) * 2;
							//l[p[i]->myPosition()]->costBuy(l[p[i]->myPosition()]->numOfAmenties()) * 2;
						cout << l[p[i]->myPosition()] << "의 현재 주인은 " << l[p[i]->myPosition()]->owner() << "이에요." << endl;
						cout << "통행료는 " << fee << "이에요. " << endl;

						//feeFree 관련 함수 없음
						if (p[i]->myMoney() >= fee)
						{

							//통행료 지불: calMoney 함수에 없음
							cout << "통행료를 지불하는 중이에요." << endl;

							cout << "건물을 인수할까요? 인수를 원하시면 1, 원하지 않으시면 2를 입력하세요. 인수 비용은 " << take_over << "이에요. " << endl;
							key = getNumber(1, 2);
							if (key == 1)
							{
								cout << "도시를 인수했어요! 인수 증서: " << l[p[i]->myPosition()]->owner() << " -> ";

								//인수: 판매 비용 2배 안맞음
								l[p[i]->myPosition()]->owner()->sellLand(l[p[i]->myPosition()],0);

								l[p[i]->myPosition()]->owner(p[i]);
								cout << l[p[i]->myPosition()]->owner() << endl;

								//인수: 구매 비용 2배 안맞음
								p[i]->buyLand(l[p[i]->myPosition()], l[p[i]->myPosition()]->numOfAmenties());
							}
						}
						else
						{
							int sum = 0;
							for (int j = 0; j < 19; j++)
							{
								if (l[j]->owner() == p[i])
								{
									if (j == IN_GWAN || j == UI_GWAN || j == YE_GWAN || j == JI_GWAN)
									{
										sum += l[j]->costBuy();
									}
									else
									{
										sum += l[j]->costBuy(l[j]->numOfAmenties());
									}
								}
							}

							if (p[i]->myMoney() + sum*0.5 < fee)
							{
								cout << p[i] << "님이 파산했습니다." << endl;
								//p[i] 제거
							}
							else
							{
								while (p[i]->myMoney() < fee)
								{
									cout << "매각을 원하는 지역의 번호를 선택하세요." << endl;
									while (true)
									{
										key = getNumber(0, 19);
										if (l[key]->owner() == p[i])
										{
											break;
										}
										else
										{
											cout << "주인이 아닌 땅을 선택했어요. 다시 입력하세요." << endl;
										}
										//매각: 판매 비용 0.5배 안맞음
										p[i]->sellLand(l[key], 1);
										l[key]->owner(NULL);
										cout << l[key] << "(을)를 매각했어요." << endl;
									}
								}
							}
						}
					}
				}
				else
				{
					if (l[p[i]->myPosition()]->owner() != p[i])
					{
						int fee = l[p[i]->myPosition()]->costFee(l[p[i]->myPosition()]->numOfAmenties());
						int take_over = l[p[i]->myPosition()]->costBuy(l[p[i]->myPosition()]->numOfAmenties()) * 2;
						cout << l[p[i]->myPosition()] << "의 현재 주인은 " << l[p[i]->myPosition()]->owner() << "이에요." << endl;
						cout << "통행료는 " << fee << "이에요. " << endl;

						//feeFree 관련 함수 없음
						if (p[i]->myMoney() >= fee)
						{
							cout << "통행료를 지불하는 중이에요." << endl;
							cout << "건물을 인수할까요? 인수를 원하시면 1, 원하지 않으시면 2를 입력하세요. 인수 비용은 " << take_over << "이에요. " << endl;
							key = getNumber(1, 2);
							if (key == 1)
							{
								cout << "도시를 인수했어요! 인수 증서: " << l[p[i]->myPosition()]->owner() << " -> ";

								//인수: 판매 비용 2배 안맞음
								l[p[i]->myPosition()]->owner()->sellLand(l[p[i]->myPosition()]);

								l[p[i]->myPosition()]->owner(p[i]);
								cout << l[p[i]->myPosition()]->owner() << endl;

								//인수: 구매 비용 2배 안맞음
								p[i]->buyLand(l[p[i]->myPosition()], l[p[i]->myPosition()]->numOfAmenties()); //..........................미처리
							}
						}
						else
						{
							int sum = 0;
							for (int j = 0; j < 19; j++)
							{
								if (l[j]->owner() == p[i])
								{
									if (j == IN_GWAN || j == UI_GWAN || j == YE_GWAN || j == JI_GWAN)
									{
										sum += l[j]->costBuy();
									}
									else
									{
										sum += l[j]->costBuy(l[j]->numOfAmenties());
									}
								}
							}

							if (p[i]->myMoney() + sum*0.5 < fee)
							{
								cout << p[i] << "님이 파산했습니다." << endl;
								//delete[] p[i];
								//p[i] 제거
							}
							else
							{
								while (p[i]->myMoney() < fee)
								{
									cout << "매각을 원하는 지역의 번호를 선택하세요." << endl;
									while (true)
									{
										key = getNumber(0, 19);
										if (l[key]->owner() == p[i])
										{
											break;
										}
										else
										{
											cout << "주인이 아닌 땅을 선택했어요. 다시 입력하세요." << endl;
										}
										//매각: 판매 비용 0.5배 안맞음
										p[i]->sellLand(l[key], 1);
										l[key]->owner(NULL);
										cout << l[key] << "(을)를 매각했어요." << endl;
									}
								}
							}
						}
					}
				}
			} while (is_double);
		}
		turn++;
	}

	return 0;
}