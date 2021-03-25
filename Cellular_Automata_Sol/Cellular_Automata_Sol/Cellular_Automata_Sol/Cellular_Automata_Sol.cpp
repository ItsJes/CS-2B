#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Automaton
{
private:
   static const int RULES_SIZE = 8;

   bool rules[RULES_SIZE];
   string thisGen;
   string extremeBit; // bit, "*" or " ", implies everywhere "outside"
   int displayWidth; // an odd number so it can be perfectlt centered

public:
   static const int MAX_DISPLAY_WIDTH = 121;
   static const int DEFAULT_DISPLAY_WIDTH = 79;
   static const int MIN_RULE = 0;
   static const int MAX_RULE = 255;

   Automaton(int rule);
   bool setRule(int rule);  // change rule set
   void resetFirstGen();
   bool setDisplayWidth(int width);
   void propagateNewGeneration();
   string toStringCurrentGen() const;

   static const string ON_STR;
   static const string OFF_STR;
};
const string Automaton::ON_STR = "*";
const string Automaton::OFF_STR = " ";

int main()
{
   int rule;
   
   // get rule from user
   do
   {
      string userInput;

      cout << "Enter Rule ( " << Automaton::MIN_RULE << " - "
         << Automaton::MAX_RULE << " ):\n";
      getline(cin, userInput);
      istringstream(userInput) >> rule;
   } 
   while (rule < Automaton::MIN_RULE || rule > Automaton::MAX_RULE);

   // create automaton with this rule and single central dot
   Automaton aut(rule);

   // noe show it
   cout << "  start" << endl;
   for (int k = 0; k < 100; k++)
   {
      cout << aut.toStringCurrentGen() << endl;
      aut.propagateNewGeneration();
   }
   cout << "  end" << endl;
}

Automaton::Automaton(int rule)
{
   if (!setRule(rule))
      setRule(0);
   resetFirstGen();
   setDisplayWidth(DEFAULT_DISPLAY_WIDTH);
}

void Automaton::resetFirstGen()
{
   // create a virtual 1 in a sea of 0s:   "     *     "
   thisGen = ON_STR;
   extremeBit = OFF_STR;
}

bool Automaton::setRule(int rule)
{
   int bitToExamine, bitResult;

   if (rule < MIN_RULE || rule > MAX_RULE)
      return false;

   for (int k = 0; k < RULES_SIZE; k++)
   {
      bitToExamine = 1 << k;
      bitResult = bitToExamine & rule;
      rules[k] = (bitResult != 0);
   }
   return true;
}

bool Automaton::setDisplayWidth(int width)
{
   // check range
   if (width < 1 || width > MAX_DISPLAY_WIDTH)
      return false;

   // requires odd parity
   if (width % 2 == 0)
      return false;

   displayWidth = width;
   return true;
}

string Automaton::toStringCurrentGen() const
{
   // amount we have to add to thisGen to give it full length of displayWidth
   int padAmount; 
   int amountToTruncEachSide;
   string displayString = "";

   padAmount = displayWidth - (int)thisGen.length();

   if (padAmount >= 0)
   {
      // we need to add padAmount/2 of extremeBit on each side of thisGen
      // to satisfy display string
      for (int k = 0; k < padAmount / 2; k++)
         displayString += extremeBit;
      displayString += thisGen;
      for (int k = 0; k < padAmount / 2; k++)
         displayString += extremeBit;
   }
   else
   {
      // truncate thisGen on both ends by padAmount/2 to fit display string
      amountToTruncEachSide = -(padAmount / 2); // or absolue value of pad/2

      for (int k = 0; k < displayWidth; k++)
         displayString += thisGen[amountToTruncEachSide + k];
   }

   return displayString;
}

void Automaton::propagateNewGeneration()
{
   int tripletVal;
   string nextGen = "";
   bool extremeBitIsOne;
   const char ON_BIT = ON_STR[0];

   // prepare by padding left and right with two extreme bits each
   thisGen = extremeBit + extremeBit + thisGen + extremeBit + extremeBit;

   /*
   we have added 4 chars to the existing generation. Now apply rule,
   which will shorten by 1 char on each side, with a net increase of
   2 chars. All the positions of nextGen[k] are determined by
   thisGen[k - 1], thisGen[k], and thisGen[k + 1]
   according to the rule set
   */

   for (int k = 1; k < thisGen.length() - 1; k++)
   {
      // turns 3 positions into an int. e.g., "** " -> 6
      tripletVal = 0;
      if (thisGen[k - 1] == ON_BIT)
         tripletVal += 4;
      if (thisGen[k] == ON_BIT)
         tripletVal += 2;
      if (thisGen[k + 1] == ON_BIT)
         tripletVal += 1;

      // now use the rule set to get the "child" of these three
      nextGen += rules[tripletVal] ? ON_STR : OFF_STR;
   }

   // extremeBit must be recomputed. First the bool that answers the question
   extremeBitIsOne = (extremeBit == OFF_STR) ? rules[0] : rules[RULES_SIZE - 1];

   // convert the bool to new character blank or *
   extremeBit = extremeBitIsOne ? ON_STR : OFF_STR;

   // finally pass the torch to the new gen
   thisGen = nextGen;
}