#include <iostream>
#include <array>	// для std::array<type, size>
#include <ctime>	// для time()
#include <cstdlib>	// для rand() и srand()

enum Rank				// класс карты
{
	RANK_2,
	RANK_3,
	RANK_4,
	RANK_5,
	RANK_6,
	RANK_7,
	RANK_8,
	RANK_9,
	RANK_10,
	RANK_JACK,			// валет
	RANK_QUEEN,			// дама
	RANK_KING,			// король
	RANK_ACE,			// туз
	MAX_RANK
};

enum Suit				// масть
{
	SUIT_CLUBS,			// треф
	SUIT_TAMBOURINE,	// бубна
	SUIT_HEARTS,		// черва
	SUIT_PIKE,			// пика
	MAX_SUIT
};

struct Card
{
	Rank cardRank;
	Suit cardSuit;
};

// получаем двухбуквенное название карты
std::string printCard(const Card& card)
{
	std::string s_Card{};
	// РАНГ КАРТЫ
	switch (card.cardRank)
	{
	case RANK_2:
		s_Card += '2';
		break;
	case RANK_3:
		s_Card += '3';
		break;
	case RANK_4:
		s_Card += '4';
		break;
	case RANK_5:
		s_Card += '5';
		break;
	case RANK_6:
		s_Card += '6';
		break;
	case RANK_7:
		s_Card += '7';
		break;
	case RANK_8:
		s_Card += '8';
		break;
	case RANK_9:
		s_Card += '9';
		break;
	case RANK_10:
		s_Card += 'T';
		break;
	case RANK_JACK:
		s_Card += 'J';
		break;
	case RANK_QUEEN:
		s_Card += 'Q';
		break;
	case RANK_KING:
		s_Card += 'K';
		break;
	case RANK_ACE:
		s_Card += 'A';
		break;
	}

	char c_Suit;
	// МАСТЬ КАРТЫ
	switch (card.cardSuit)
	{
	case SUIT_CLUBS:
		s_Card += 'C';
		break;
	case SUIT_TAMBOURINE:
		s_Card += 'T';
		break;
	case SUIT_HEARTS:
		s_Card += 'H';
		break;
	case SUIT_PIKE:
		s_Card += 'P';
		break;
	}

	return s_Card;
}

// выводим колоду
void printDeck(const std::array<Card, 52>& deck)
{
	for (const auto& card : deck)
		std::cout << "Card #" << &card - &deck[0] + 1 << " -> " <<
			printCard(card) << '\n';
}

// меняем карты
void swapCard(Card& card1, Card& card2)
{
	Card midCard = card1;	// сохраняем card1
	card1 = card2;			// card2 присваиваем card1
	card2 = midCard;		// card1 присваиваем card2
}

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 0.999) + min);
}

// тасуем колоду
void shuffleDeck(std::array<Card, 52>& deck)
{
	for (int i(0); i < deck.size(); ++i)
	{
		int randomCard(getRandomNumber(0, 51));
		swapCard(deck[i], deck[randomCard]);
	}
}

// получаем значение карты
int getCardValue(const Card &card)
{
	switch (card.cardRank)
	{
	case RANK_2:		return 2;
	case RANK_3:		return 3;
	case RANK_4:		return 4;
	case RANK_5:		return 5;
	case RANK_6:		return 6;
	case RANK_7:		return 7;
	case RANK_8:		return 8;
	case RANK_9:		return 9;
	case RANK_10:		return 10;
	case RANK_JACK:		return 10;
	case RANK_QUEEN:	return 10;
	case RANK_KING:		return 10;
	case RANK_ACE:		return 11;
	}

	return 0;
}

enum HitOrStand
{
	HIT,	// взять карту
	STAND	// удержаться
};

// играем
bool playBlackjack(const std::array<Card, 52> &deck)
{
	using std::cout;
	using std::cin;
	cout << "*** Blackjack ***\n";
	// указатель на первую карту
	const Card* cardPtr(&deck[0]);
	std::int16_t dealerScore(0);
	std::int16_t playerScore(0);

	// дилер получает карту
	dealerScore += getCardValue(*cardPtr++);

	// игрок получает 2 карты
	playerScore += getCardValue(*cardPtr++);
	playerScore += getCardValue(*cardPtr++);

	cout << "Player's score -> " << playerScore << '\n';
	// hit - взять, stand - удержаться
	while (1)
	{
		cout << "Hit - 0 or stand - 1 -> ";
		int hos;
		cin >> hos;

		// если игрок удержался
		if (hos == static_cast<int>(STAND)) {
			cout << "The player's turn is over.\n";
			break;
		}
		// если игрок берёт
		else if (hos == static_cast<int>(HIT))
		{
			playerScore += getCardValue(*cardPtr);
			++cardPtr;
			cout << "Player's score -> " << playerScore << '\n';

			// если больше 21 очка, то игрок проиграл
			if (playerScore > 21)
				return false;
		}
	}

	cout << "Dealer's score -> " << dealerScore << '\n';

	// ход дилера
	while (dealerScore < 17)
	{
		dealerScore += getCardValue(*cardPtr);
		++cardPtr;
		cout << "Dealer's score -> " << dealerScore << '\n';
	}

	// > 21, дилер проиграл
	if (dealerScore > 21)
		return true;

	return (playerScore > dealerScore);
}

int main()
{
	using namespace std;
	srand(time(0));
	rand();

	// массив с колодой карт
	array<Card, 52> deck;

	// последовательная колода карт
	int curCard(0);
	for (int rank(0); rank < MAX_RANK; ++rank)
		for (int suit(0); suit < MAX_SUIT; ++suit)
		{
			deck[curCard].cardRank = static_cast<Rank>(rank);
			deck[curCard].cardSuit = static_cast<Suit>(suit);
			++curCard;
		}

	// тасуем колоду карт
	shuffleDeck(deck);

	bool resultGame(playBlackjack(deck));
	if (!resultGame)
		cout << "*** The player lost. ***\n";
	else
		cout << "*** The player won. ***\n";

	return 0;
}