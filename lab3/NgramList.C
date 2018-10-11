#include <iostream>
#include <string>
#include <vector>
#include "WordList.h"
#include "NgramList.h"
#include <stdlib.h>
#include <map>

using namespace std;

//way to more effectively store ngrams
map<string, int> ngramStorage;				//https://www.hackerrank.com/challenges/cpp-maps/problem

//way to more effectively sort ngrams
multimap<int, string> ngramStorageSorted;		

/*
 * NgramList
 *
 * takes as input the size of the ngrams to be built and the list  
 * of words to build the ngrams from and builds a linked list of 
 * ngrams.
 *
 * param: int ngramSz - size of the ngram
 * param: const WordList & wl - list of the words use
 */
NgramList::NgramList(int ngramSz, const WordList & wl)
{
   this->ngramSz = ngramSz;
   WordList::const_iterator p;
   first = NULL;
   p = wl.begin();
   while (p != wl.end())
   {
      std::string ngram = getNextNgram(p, wl.end());
      p++;
      if (!ngram.empty()) insertNgram(ngram);
   }
}

/*
 * NgramList destructor
 *
 * automatically called when NgramList object goes out of scope
 * deletes the linked list
 */
NgramList::~NgramList()
{
   Ngram_t * nextNgram;
   while (first != NULL)
   {
      nextNgram = first->next;
      //free(first);
	  delete first;			//THE WRONG MEMORY FREEING METHOD WAS USED BECAUSE first IS AN OBJECT NOT A VARIABLE
      first = nextNgram;
   }
}

/*
 * getNextNgram
 *
 * takes as input an iterator that points to the word to start the
 * ngram and an iterator that points to the end of the word list
 * and builds and returns the ngram. returns empty string if no
 * ngram can be built, for example, if there are insufficient words
 * to build one.
 *
 * param: WordList::const_iterator start - points to first word in ngram
 * param: WordList::const_iterator end - points to end of word list
 * return: std::string - returns ngram or empty string
 */
std::string NgramList::getNextNgram(WordList::const_iterator start, 
                                    WordList::const_iterator end)
{
   int i, len;
   string ngram = "";
   for (i = 0; i < ngramSz && start != end; i++)
   {
      std::string word = (*start);
      //see if the string ends with punctuation
      //don't create ngrams that continue after punctuation
      if (!isalpha(word[word.length()-1]) && i < ngramSz - 1) return "";

      //take off all ending punctuation
      len = word.length() - 1;
      while (len >= 0 && !isalpha(word[len])) 
      {
         //remove last character
         word = word.substr(0, word.length() - 1);
         len--;
      }
      if (len < 0) return "";  //give up

      //is the first word in the ngram?
      if (ngram == "") ngram = word;
      else ngram = ngram + " " + word;

      start++;
   }

   //could we build a long enough ngram?
   if (i < ngramSz) return "";

   //take off beginning punctuation
   while (ngram.length() > 0 && !isalpha(ngram[0])) 
      ngram = ngram.substr(1, ngram.length());
   return ngram;
}

/*
 * insertNgram
 *
 * checks to see if Ngram already exists. If it does then count gets
 * incremented by one. If it does not then the Ngram is put into the 
 * map of Ngrams.
 *
 * param: std::string s - ngram to be inserted
 * return: none
 */
void NgramList::insertNgram(std::string s)
{
	/*checks to see if Ngram already exists*/
	std::map<string, int>::iterator iter = ngramStorage.find(s);		//https://www.hackerrank.com/challenges/cpp-maps/problem
	
	/*it does exist so, count is incremented by one*/
	if(iter != ngramStorage.end()){iter->second++;}						//https://www.hackerrank.com/challenges/cpp-maps/problem
	
	/*it does not exist so, Ngram is added to map and count is one*/
	ngramStorage.insert(make_pair(s, 1));								//https://www.hackerrank.com/challenges/cpp-maps/problem
}

/*
 * sortByCount
 *
 * sorts the Ngrams in the map by count into a multimap 
 *
 * param: none
 * return: none
 */
void NgramList::sortByCount()
{
	//http://www.cplusplus.com/reference/map/multimap/
	for (auto iter1 = ngramStorage.begin(); iter1 != ngramStorage.end(); ++iter1)
	{
		ngramStorageSorted.insert(make_pair(iter1->second, iter1->first));
	}
	
}

/*
 * operator<<
 *
 * prints the ngrams from the multimap
 *
 * param: std::ostream & os - output stream to direct the output to
 * param: const NgramList & nglst - ngram list object
 * return: std::ostream & - output stream
 */  
std::ostream& operator<<(std::ostream& os, const NgramList & nglst)
{
    cout << "List of " << nglst.ngramSz << " word ngrams and counts\n";
    cout << "--------------------------------\n";
	
	for (auto iter2 = ngramStorageSorted.rbegin(); iter2 != ngramStorageSorted.rend(); ++iter2)
	{
		std::cout << iter2->second << " " << iter2->first << "\n";
	}
}


/*-------------------------ORIGINAL METHOD--------------------------
 * insertNgram
 *
 * looks for the ngram to be inserted. If it is already in
 * the linked list,  it increments the count. If not, it 
 * inserts it into the linked list.
 *
 * param: std::string s - ngram to be inserted
 * return: none
 *
void NgramList::insertNgram(std::string s)
{
   Ngram_t * ptr = first;
   Ngram_t * newNode = new Ngram_t();
   newNode->ngram = s;
   newNode->count = 1;

   while (ptr != NULL)
   {
      //s already in list
      if (ptr->ngram == s) 
      {
         ptr->count++;
		 delete newNode;	//memory allocated and not in use
         return;
      }
      ptr = ptr->next;
   }
   //insert in front of list
   newNode->next = first;
   first = newNode;
}------------------------------------------------------------------*/

/*-------------------------ORIGINAL METHOD---------------------------
 * sortByCount
 *
 * performs a bubble sort on the linked list of ngrams, sorting the
 * nodes in the list by the count
 *
 * param: none
 * return: none (modfied private linked list)
 
void NgramList::sortByCount()
{
   Ngram_t * ptr = first;
   Ngram_t * ptr1;
   Ngram_t * ptr2;
   int tcount;
   string tngram;

   while (ptr != NULL)
   {
      ptr1 = first; 
      ptr2 = ptr1->next;
      while (ptr2 != NULL) 
      {
         if (ptr2->count > ptr1->count)
         {
            tcount = ptr1->count;
            tngram = ptr1->ngram;
            ptr1->count = ptr2->count;
            ptr1->ngram = ptr2->ngram; 
            ptr2->count = tcount;
            ptr2->ngram = tngram;
         }
         ptr1 = ptr2; 
         ptr2 = ptr2->next;
      }
      ptr = ptr->next;
   }
}--------------------------------------------------------------------*/

/*-------------------------ORIGINAL METHOD----------------------------
 * operator<<
 *
 * prints the list of ngrams
 *
 * param: std::ostream & os - output stream to direct the output to
 * param: const NgramList & nglst - ngram list object
 * return: std::ostream & - output stream
 *  
std::ostream& operator<<(std::ostream& os, const NgramList & nglst)
{
   cout << "List of " << nglst.ngramSz << " word ngrams and counts\n";
   cout << "--------------------------------\n";
   NgramList::Ngram_t * ptr = nglst.first;
   while (ptr != NULL)
   {
      cout << ptr->ngram << ", " << ptr->count << endl;
      ptr = ptr->next;
   } 
   return os;
}------------------------------------------------------------------*/



