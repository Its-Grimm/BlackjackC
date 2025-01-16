#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_OF_DECKS 6
#define ONE_DECK_SIZE 52
#define MAX_DECK_SIZE (ONE_DECK_SIZE * NUM_OF_DECKS) // 6 decks = 312 cards total
#define MAX_CARDS_IN_HAND 22

typedef struct {
   int number;
   char suit;
   int value;
} Card;


// Prototypes + variables for stack implementation
void show_hand(Card *hand, int hand_size);
void deal_to(Card *hand_to_deal_to, Card card_to_push, int *hand_size, int max_hand_size, int *total_cards_pushed);

// Custom card function prototypes
void shuffle_deck(Card *deck);
int get_hand_value(Card *hand, int current_hand_size);
void initialize_cards(Card *cards, int size);
void swap_card(Card *hand_to_swap_card, int current_hand_size, Card card_to_swap_in);

// Prototypes for formatting card output
void display_cards(char buffer[20], int card_number, char card_suit, int count);
void format_card_number(int card_number, char *buffer);
void format_card_suit(char card_suit, char *buffer);


int main(void) {
   srand(time(0));

   Card deck[MAX_DECK_SIZE];
   Card player_hand[MAX_CARDS_IN_HAND];
   Card dealer_hand[MAX_CARDS_IN_HAND];
   initialize_cards(deck, MAX_DECK_SIZE);
   initialize_cards(player_hand, MAX_CARDS_IN_HAND);
   initialize_cards(dealer_hand, MAX_CARDS_IN_HAND);


   const int card_numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
   const char card_suits[] = {'C', 'S', 'H', 'D'};
   const int card_value[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

   // Done: Have each card randomly selected from the numbers and suits as a
   // card struct and then added to a stack of card type, making up the
   // deck-to-deal or loop through all possibilities and then shuffle.
   Card card;
   int temp = 0;
   int card_count = 0;
   int amount_of_numbers = 13;
   int amount_of_suits = 4;
   for (int i = 0; i < NUM_OF_DECKS; i++) {
      // Creates one full 52 card deck
      for (int j = 0; j < amount_of_numbers; j++) {
	 for (int k = 0; k < amount_of_suits; k++) {
	    card.number = card_numbers[j];
	    card.suit = card_suits[k];
	    card.value = card_value[j];
	    deal_to(deck, card, &card_count, MAX_DECK_SIZE, &temp);
	 }
      }
   }
   shuffle_deck(deck);


   // Done: Implement gameloop, dealing, player hands, and dealer hands
   int cards_dealt = 0;
   int user_input = -1;
   bool keep_playing = true;
   while (keep_playing) {
      printf("[0] to Quit\n[1] to Play\nChoice:\t");
      scanf("%d", &user_input);
      if (user_input != 1) {
	 keep_playing = false;
	 return 0;
      }
      printf("\n\n");

      // printf("Total cards dealt: %d\n", cards_dealt);
      // printf("Cards left in deck: %d\n", (MAX_DECK_SIZE - cards_dealt));

      // Done: Shuffle cards when a certain amount of cards are left (default 50)
      if ((MAX_DECK_SIZE - cards_dealt) <= 50) {
	 printf("Shuffling!\n\n");
	 shuffle_deck(deck);
	 cards_dealt = 0;
      }

      int player_cards_dealt = 0;
      int dealer_cards_dealt = 0;

      // Initial deal
      deal_to(player_hand, deck[cards_dealt], &player_cards_dealt, MAX_CARDS_IN_HAND, &cards_dealt);

      Card dealer_flipped_card;
      dealer_flipped_card.number = 0;
      dealer_flipped_card.suit = 'F';
      dealer_flipped_card.value = 0;
      Card flipped_card = deck[cards_dealt];
      deal_to(dealer_hand, dealer_flipped_card, &dealer_cards_dealt, MAX_CARDS_IN_HAND, &cards_dealt);

      deal_to(player_hand, deck[cards_dealt], &player_cards_dealt, MAX_CARDS_IN_HAND, &cards_dealt);
      deal_to(dealer_hand, deck[cards_dealt], &dealer_cards_dealt, MAX_CARDS_IN_HAND, &cards_dealt);

      printf("Cards in players hand:");
      show_hand(player_hand, player_cards_dealt);
      printf("Cards in dealers hand:");
      show_hand(dealer_hand, dealer_cards_dealt);

      // printf("Players points: %d\n", get_hand_value(player_hand, player_cards_dealt));
      // printf("Dealers points: %d\n\n\n", get_hand_value(dealer_hand, dealer_cards_dealt));

      // If player hits and stays under 22
      int stand = 0;
      int hit = 1;
      int stand_or_hit = -1;
      while (!(get_hand_value(player_hand, player_cards_dealt) > 21) && stand_or_hit != stand) {
	 printf("\n[0] to Stand\n[1] to Hit\nChoice:\t");
	 scanf("%d", &stand_or_hit);
	 printf("\n\n");

	 if (stand_or_hit == hit) {
	    deal_to(player_hand, deck[cards_dealt], &player_cards_dealt, MAX_CARDS_IN_HAND, &cards_dealt);

	    printf("Cards in players hand:");
	    show_hand(player_hand, player_cards_dealt);
	 }
      }

      // Making dealer draw when lower than 17
      swap_card(dealer_hand, dealer_cards_dealt, flipped_card);
      while (get_hand_value(dealer_hand, dealer_cards_dealt) < 17) {
	 printf("Dealers hand < 17, drawing again!");
	 deal_to(dealer_hand, deck[cards_dealt], &dealer_cards_dealt, MAX_CARDS_IN_HAND, &cards_dealt);
	 show_hand(dealer_hand, dealer_cards_dealt);
      }

      // Results
      printf("\n~~~Results~~~\n");
      printf("Cards in players hand:");
      show_hand(player_hand, player_cards_dealt);
      printf("Cards in dealers hand:");
      show_hand(dealer_hand, dealer_cards_dealt);

      int player_hand_value = get_hand_value(player_hand, player_cards_dealt);
      int dealer_hand_value = get_hand_value(dealer_hand, dealer_cards_dealt);

      if (player_hand_value > 21 || dealer_hand_value > 21) {
	 if (player_hand_value > 21 && dealer_hand_value > 21) {
	    printf("Both busted, bets returned!\n");
	 }
	 else if (player_hand_value > 21) {
	    printf("Player busts with %d, Dealer wins!\n", player_hand_value);
	 }
	 else if (dealer_hand_value > 21) {
	    printf("Dealer busts with %d, Player wins!\n", dealer_hand_value);
	 }
      }
      else {
	 if (player_hand_value > dealer_hand_value) {
	    printf("Player wins the hand %d to %d!\n", player_hand_value, dealer_hand_value);
	 }
	 else if (player_hand_value < dealer_hand_value) {
	    printf("Dealer wins the hand %d to %d!\n", dealer_hand_value, player_hand_value);
	 }
	 else {
	    printf("Hands equal, bets returned!\n");
	 }
      }
      printf("\n\n");
   }


   // Todo: Have a method responsible for displaying updates to hands/gamestate


   // Todo: Implement card count +/- and Basic Strategy display for player


   // Todo: Implement betting, money, winrate, and other data retention (stored in file)


   // Todo: Implement bot that plays at different levels of ability
   //         -Plays with the best of limited knowledge
   //         -Plays using Basic Strategy perfectly
   //         -Plays using the above + card counting
   //         -Will need gameplay tracker to see how each bot is doing


   // Todo: Have game options stored in GameOpts.txt, which the player can
   //       change and view in the game settings. These options can include:
   //         -Max Deck Size (Default is 6 decks (312 cards)). Can go up to 8 decks (416 cards)
   //         -When the dealer shuffles (Default is when there are 50 or less cards left)
   //         -How many players at the table (Default 1, player & dealer)
   //         -Display hints (showing Basic Strategy, +/- count, etc) (Default is off)
   //         -Bet min and max limit (Default is Minimum $2 and Maximum $500)
   //         -Blackjack hand win payout (Default 3:2, can be 6:5 to have worse odds)


   return 0;
}


void show_hand(Card *hand, int hand_size) {
   printf("\n");
   if (hand_size == -1) {
      printf("\nUnderflow!!");
   }
   else {
      for (int i = hand_size - 1; i >= 0; --i) {
	 if (hand[i].number == 0 && hand[i].suit == 'F') {
	    printf("Flipped Card\n");
	 }
	 else {
	    char buffer[40] = {0};
	    display_cards(buffer, hand[i].number, hand[i].suit, i);
	    printf("%s\n", buffer);
	 }
      }
   }
   printf("\n");
}


void deal_to(Card *hand, Card card, int *current_hand_size, int max_hand_size, int *total_cards_pushed) {
   if (*current_hand_size >= max_hand_size) {
      printf("\nOverflow!!");
   }
   else {
      hand[*current_hand_size] = card;

      char buffer[40] = {0};
      display_cards(buffer, hand[*current_hand_size].number, hand[*current_hand_size].suit, *current_hand_size);
      // printf("Pushed %s from deck to hand.\n", buffer);
      *current_hand_size = *current_hand_size + 1;
      *total_cards_pushed = *total_cards_pushed + 1;
   }
}


void shuffle_deck(Card *deck) {
   // Shuffles deck of cards (array) using the Fisher-Yates method
   Card temp;
   int rand_val;
   for (int i = MAX_DECK_SIZE - 1; i > 0; i--) {
      rand_val = (rand() % (i + 1));

      temp = deck[i];
      deck[i] = deck[rand_val];
      deck[rand_val] = temp;
   }
}


int get_hand_value(Card *hand, int current_hand_size) {
   int points = 0;
   for (int i = 0; i < current_hand_size; i++) {
      if (hand[i].value == -1) {
	 printf("Card %d in hand has no value!!!", i);
      }
      else {
	 points += hand[i].value;
      }
   }
   return points;
}


void initialize_cards(Card *cards, int size) {
   for (int i = 0; i < size; i++) {
      cards[i].number = 0;
      cards[i].suit = '\0';
      cards[i].value = 0;
   }
}


void swap_card(Card *hand_to_swap_card, int current_hand_size, Card card_to_swap_in) {
   for (int i = 0; i < current_hand_size; i++) {
      if (hand_to_swap_card[i].number == 0 && hand_to_swap_card[i].suit == 'F') {
	 hand_to_swap_card[i] = card_to_swap_in;
      }
   }
}


void display_cards(char output[40], int card_number, char card_suit, int count) {
   char number[10];
   char suit[15];

   format_card_number(card_number, number);
   format_card_suit(card_suit, suit);
   snprintf(output, 25, "%s of %s", number, suit);
}


void format_card_number(int card_number, char *buffer) {
   switch (card_number) {
      case 1:
	 snprintf(buffer, 10, "Ace");
	 break;
      case 11:
	 snprintf(buffer, 10, "Jack");
	 break;
      case 12:
	 snprintf(buffer, 10, "Queen");
	 break;
      case 13:
	 snprintf(buffer, 10, "King");
	 break;
      default:
	 snprintf(buffer, 10, "%d", card_number);
	 break;
   }
}


void format_card_suit(char card_suit, char *buffer) {
   switch (card_suit) {
      case 'S':
	 snprintf(buffer, 15, "Spades");
	 break;
      case 'C':
	 snprintf(buffer, 15, "Clubs");
	 break;
      case 'D':
	 snprintf(buffer, 15, "Diamonds");
	 break;
      case 'H':
	 snprintf(buffer, 15, "Hearts");
	 break;
      case 'F':
	 snprintf(buffer, 15, "Flipped");
	 break;
      default:
	 snprintf(buffer, 15, "Unknown");
	 break;
   }
}
