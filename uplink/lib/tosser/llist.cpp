
#ifndef _included_tosser_llist
#define _included_tosser_llist

#include <cassert>

#include "tosser.h"

template <class T>
LList <T>::LList ()
{

    first = NULL;
    last = NULL;
    numitems = 0;
    previous = NULL;
    previousindex = -1;

}

template <class T>
LList<T>::~LList ()
{
    Empty();
}

/**
 * Appends data to LList
 * @tparam T Type of LList
 * @param newdata Data to insert
 */
template <class T>
void LList<T>::PutData ( const T &newdata )
{

    PutDataAtEnd ( newdata );
	
}

/**
 * Appends data to LList
 * @tparam T Type of LList
 * @param newdata Data to insert
 */
template <class T>
void LList<T>::PutDataAtEnd ( const T &newdata )
{

    // Create the new data entry
    LListItem <T> *li = new LListItem <T> ();
    li->data = newdata;
    li->next = NULL;
    ++numitems;

    if ( last == NULL ) {

        // No items added yet - this is the only one

        li->previous = NULL;
        first = li;
        last = li;

    }
    else {

        // Add the data onto the end
        last->next = li;
        li->previous = last;
        last = li;		

    }

    previousindex = -1;
    previous = NULL;

}

/**
 * Prepends data to LList
 * @tparam T Type of LList
 * @param newdata Data to insert
 */
template <class T>
void LList<T>::PutDataAtStart ( const T &newdata )
{

    // Create the new data entry
    LListItem <T> *li = new LListItem <T> ();
    li->data = newdata;
    li->previous = NULL;
    ++numitems;

    if ( last == NULL ) {

        // No items added yet - this is the only one

        li->next = NULL;
        first = li;
        last = li;

    }
    else {

        // Add the data onto the start
        first->previous = li;
        li->next = first;
        first = li;

    }

    previousindex = -1;
    previous = NULL;

}

/**
 * Inserts data at index of LList
 * @tparam T Type of LList
 * @param newdata Data to insert
 * @param index Index to insert data at
 */
template <class T>
void LList <T>::PutDataAtIndex ( const T &newdata, int index )
{

    if ( index == 0 ) {
		
        PutDataAtStart ( newdata );
		
    }
    else if ( index == numitems ) {
		
        PutDataAtEnd ( newdata );

    }
    else {

        // Create the new data entry
        LListItem <T> *li = new LListItem <T> ();
        li->data = newdata;		

        LListItem <T> *current = first;

        for ( int i = 0; i < index - 1; ++i ) {

            if ( !current )	return;
            current = current->next;

        }

        if ( !current )	return;
		
        li->previous = current;
        li->next = current->next;
        if ( current->next ) current->next->previous = li;
        current->next = li;		
        ++numitems;
        previousindex = -1;
        previous = NULL;
		
    }
		
}

/**
 * Gets the number of LList items
 * @tparam T Type of LList
 * @return Number of items in LList
 */
template <class T>
int LList <T>::Size ()
{

    return numitems;

}

/**
 * Returns data from given index in LList
 * @tparam T Type of LList
 * @param index Index to get data from
 * @return The data at given index
 */
template <class T>
T LList <T>::GetData ( int index )
{

    if ( previous && previousindex != -1 && index == previousindex + 1 ) {

        // Last access was to the previous index, so we can access 
        // this one much faster
		
        previous = previous->next;
        previousindex++;

        if ( previous == NULL ) return (T) 0;
        else return previous->data;

    }
    else if ( previous && previousindex != -1 && index == previousindex ) {

        // Last access was to this index, so we can 
        // access it directly

        return previous->data;

    }
    else {

        // We must find the data the long way

        LListItem <T> *current = first;

        for ( int i = 0; i < index; ++i ) {

            if ( current == NULL )
                return (T) 0;

            current = current->next;

        }

        previous = current;	

        if ( current == NULL ) {
            previousindex = -1;
            if (typeid(T) == typeid(string)) {
                return T();
            } else {
                return (T) 0;
            }
        }
        else {
            previousindex = index;
            return current->data;
        }

    }

}

/**
 * Returns data from given index in LList
 * @tparam T Type of LList
 * @param index Index to get data from
 * @return The data at given index
 */
template <class T>
T LList <T> :: operator [] (int index)
{
    
    return GetData (index);    

}

/**
 * Checks if the index exists in the LList
 * @tparam T Type of the LList
 * @param index Index to check if exists
 * @return `true` if index exists
 */
template <class T>
bool LList <T>::ValidIndex ( int index )
{

    return ( index >= 0 && index < numitems );

}

/**
 * Deletes all items in LList
 * @tparam T Type of the LList
 */
template <class T>
void LList <T>::Empty ()
{
/*
    LListItem <T> *next = 0;
    for (LListItem <T> *p = first; p; p = next) {
        next = p->next;
        delete p;
    }
*/

    LListItem <T> *current = first;
    while ( current ) {
        LListItem <T> *next = current->next;
        delete current;
        current = next;
    }

    first = NULL;
    last = NULL;
    numitems = 0;
    previous = NULL;
    previousindex = -1;
    
}

/**
 * Deletes an index in an LList
 * @tparam T Type of the LList
 * @param index Index to delete in LList
 */
template <class T>
void LList <T>::RemoveData ( int index )
{

    LListItem <T> *current = first;

    for ( int i = 0; i < index; ++i ) {

        if ( current == NULL )
            return;

        current = current->next;

    }

    if ( current == NULL )
        return;

    if ( current->previous == NULL )
        first = current->next;

    if ( current->next == NULL )
        last = current->previous;

    if ( current->previous ) current->previous->next = current->next;
    if ( current->next )     current->next->previous = current->previous;

    delete current;

    previousindex = -1;  
    previous = NULL;

    --numitems;
        
}

/**
 * Finds where the data is in an LList
 * @tparam T Type of the LList
 * @param data Data to look for
 * @return The index of the data in the LList, `-1` if it doesn't exist
 */
template <class T>
int LList <T>::FindData ( const T &data )
{

    for ( int i = 0; i < Size (); ++i )
        if ( GetData (i) == data )
            return i;

    return -1;

}

template <class T>
LListItem <T>::LListItem ()
{

    next = NULL;
    previous = NULL;

}

template <class T>
LListItem <T>::~LListItem ()
= default;

#endif
