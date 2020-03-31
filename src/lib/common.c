typedef unsigned int u32;

int array_teq(u32* arr1, u32 len1, u32* arr2, u32 len2) 
{
  if(len1 != len2)
    return 0;
  else {
    int eq = 1;
    for(int i = 0; i < len1; i++)
      eq = eq && (arr1[i] == arr2[i]);
    return eq; 
  }
};