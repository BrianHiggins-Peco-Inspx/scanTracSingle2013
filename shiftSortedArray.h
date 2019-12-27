//void shiftSortedArray(sortedArray, filterLength, oldVal, newVal);
/*
 *  Inline code for fast median filtering.
 *	Shifts sorted vector 1 place further in an array.
 *	Removes old value, then adds new value to the vector.
 *  Input:
 *		sortedArray - old version of sorted array
 *		filterLength - length of median filter
 *		oldVal - value to be removed
 *		newVal - value to be added
 */
{
	int j,k,insertIndex;
	short *ptmp, *ptmp2;		
	int length1 = filterLength - 1;
	
	// remove old value from sorted list
	ptmp = sortedArray;
	for (j=0; j<filterLength; j++) {	// travel vector from the left to find old value 
		if ((*ptmp++) == oldVal) {		// if found,															
			ptmp2 = ptmp-1;
			for (k=j; k<length1; k++) *ptmp2++ = *ptmp++;   // shift left list items so that removed one is replaced
			break;
		}
	}
	// insert new value in sorted list
	ptmp = sortedArray;
	insertIndex = length1;
	for (j=0; j<length1; j++) {			// travel vector to find insertion point of the new value
		if ((*ptmp++) > newVal) {		// if found, insert it, and 
			insertIndex = j;
			break;
		}
	}
	ptmp = sortedArray + length1;
	ptmp2 = ptmp-1;
	
	for (j=length1; j>insertIndex; j--) *ptmp-- = *ptmp2--;   // shift right list items to make a place for the new one
	*ptmp = newVal;											  // insert new value

}
