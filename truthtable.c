#include<stdio.h>
#include<math.h>
#include<string.h>

/*
  NEGATION: ~p
  CONJUNCTION: p^q
  DISJUNCTION: pvq
  IMPLICATION: p>q
  EQUIVALENCE: p=q
*/
const char NEGATION_SYMB = '~';
const char CONJUNCTION_SYMB = '^';
const char DISJUNCTION_SYMB = 'v';
const char IMPLICATION_SYMB = '>';
const char EQUIVALENCE_SYMB = '=';

char isSymbol(char c){
	return c == NEGATION_SYMB || c == CONJUNCTION_SYMB || c == DISJUNCTION_SYMB || c == IMPLICATION_SYMB || c == EQUIVALENCE_SYMB;
}

char contains(char* s, char c){
	int length = sizeof(s)/sizeof(s[0]);
  for(int i = 0; i < length; i++)
    if(s[i] == c)
      return 1;
  return 0;
}
 
int getVariableIndex(char* s, char c){
	int length = sizeof(s)/sizeof(s[0]);
  for(int i = 0; i < length; i++)
    if(s[i] == c)
      return i;
  return -1;
}
 
char getVariableValueFromName(char c, char* varsName, char* values){
  switch(c){
    case 'V': return 1;
    case 'F': return 0;
    default: return values[getVariableIndex(varsName, c)];
  }
}
 
void removeFromArray(char* array, int index){
	int length = sizeof(array)/sizeof(array[0]);
  for(int i = index; i < length - 1; i++){
    array[i] = array[i + 1];
  }
}
 
void resolveParenthesis(char* expr, char* varsName, char* values){
  int size = sizeof(expr)/sizeof(expr[0]);

  for(int i = 0; i < size; i++){
    if(expr[i] == NEGATION_SYMB){
      char affectedChar = expr[i + 1];
      char value = getVariableValueFromName(affectedChar, varsName, values);
            
      expr[i] = !value ? 'V' : 'F';
            
      removeFromArray(expr, i + 1);
            
      resolveParenthesis(expr, varsName, values);
      break;
    }
  }
   
  for(int i = 0; i < size; i++){
    if(isSymbol(expr[i]) == 1 && expr[i] != NEGATION_SYMB){
      char lastValue = getVariableValueFromName(expr[i - 1], varsName, values);
      char nextValue = getVariableValueFromName(expr[i + 1], varsName, values);
      char result;
      
      if(expr[i] == IMPLICATION_SYMB) result = !lastValue | nextValue;
      else if(expr[i] == CONJUNCTION_SYMB) result = lastValue & nextValue;
      else if(expr[i] == DISJUNCTION_SYMB) result = lastValue | nextValue;
      else if(expr[i] == EQUIVALENCE_SYMB) result = lastValue == nextValue;

      expr[i - 1] = result ? 'V' : 'F';
      removeFromArray(expr, i);
      removeFromArray(expr, i);
           
      resolveParenthesis(expr, varsName, values);
      break;
    }
  }
}
 
void resolveExpression(char expr[], char* varsName, char* values){
  int size = strlen(expr);
   
  int lastOpenParenInd = -1;
  for(int i = 0; i < size; i++){
      if(expr[i] == '('){
        lastOpenParenInd = i;
      }else if(expr[i] == ')'){
          char localExpression[i - lastOpenParenInd];
           
          strncpy(localExpression, expr + lastOpenParenInd + 1, i - lastOpenParenInd - 1);
           
          resolveParenthesis(localExpression, varsName, values);
          expr[lastOpenParenInd] = localExpression[0];
           
          for(int j = i; j >= lastOpenParenInd + 1; j--){
            removeFromArray(expr, j);
          }
           
          resolveExpression(expr, varsName, values);
          break;
      }
  }
  
  resolveParenthesis(expr, varsName, values);
}
 
int main() {
  /*
    NEGATION: ~p
    CONJUNCTION: p^q
    DISJUNCTION: pvq
    IMPLICATION: p>q
    EQUIVALENCE: p=q
  */
  char expr[] = "~(p>q)^r";
 
  int exprSize = strlen(expr);
  
  char variableNames[8];
  
  int varCount = 0;
   
  for(int i = 0; i < exprSize; i++){
    if(expr[i] >= 'a' && expr[i] <= 'z' && isSymbol(expr[i]) == 0){
      if(!contains(variableNames, expr[i])){
        variableNames[varCount] = expr[i];
        varCount++;
      }
    }
  }
   
  for(int i = 0; i < varCount; i++){
    printf("%c  ", variableNames[i]);
  }
   
  printf("|  %s\n", expr);
  printf("-----------------------------\n");
   
  unsigned char count = 0;
   
  for(int i = 0; i < pow(2, varCount); i++){
    char bits[varCount];
       
    for(int j = 0; j < varCount; j++){
      bits[j] = (count >> j) & 0x01;
      printf("%c  ", bits[j] ? 'V' : 'F');
    }
       
    char exprCopy[exprSize];
    strcpy(exprCopy, expr);
    resolveExpression(exprCopy, variableNames, bits);
       
    printf("|  %c\n", exprCopy[0]);
       
    count++;
  }
}
