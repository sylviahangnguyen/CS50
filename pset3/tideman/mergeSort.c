void mergeSort(int vic[], pair pair[], int start, int end)
{
    int middle = (end + start) / 2;
    mergeSort(vic, pair, start, middle);
    mergeSort(vic, pair, middle + 1, end);
    merge(vic, pair, start, middle, end);
}

void merge(int vic[], pair pair[], int start, int middle, int end)
{
    int i, j, k;

    /* Create temporary array*/
    int left = middle - start + 1;
    int right = end - middle;
    int left_vic[left];
    int right_vic[right];
    pair sorted_pairs[end + 1];


    /* Move data from arr[] into 1st_half[] and 2nd_half[] */
    for (i = 0; i < left; i ++)
    {
        left_vic[i] = vic[start + i];
    }
    for (j = 0; j < right; j ++)
    {
        right_vic[j] = vic[middle + 1 + j];
    }

    i = 0; j = 0; k = start;
    while (i < left && j < right)
    {
        if (left_vic[i] >= right_vic[j])
        {
            vic[k] = left_vic[i];
            sorted_pairs[k] = pair[start + i];
            i ++;
        }
        else
        {
            vic[k] = right_vic[j];
            sorted_pairs[k] = pair[middle + 1 + j];
            j ++;
        }
        k ++;
    }

    while (i < left)
    {
        vic[k] = left_vic[i];
        sorted_pairs[k] = pair[start + i];
        i ++;
    }

    while (j < right)
    {
        vic[k] = right_vic[j];
        sorted_pairs[k] = pair[middle + 1 + j];
        j ++;
    }
}
