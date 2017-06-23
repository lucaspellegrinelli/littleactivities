#
include < stdio.h > #include < math.h > #include < string.h >

  typedef unsigned char bool;#
define true 1# define false 0

typedef struct Variable {
  char name;
  int index;
}
Variable;

bool contains(Variable * s, int varCount, char c) {
  for (int i = 0; i < varCount; i++)
    if (s[i].name == c)
      return true;
  return false;
}

int getVariableIndex(Variable * s, int varCount, char c) {
  for (int i = 0; i < varCount; i++)
    if (s[i].name == c)
      return i;
  return -1;
}

bool getVariableValueFromName(char c, Variable * vars, int varCount, bool * values) {
  switch (c) {
  case 'V':
    return true;
  case 'F':
    return false;
  default:
    return values[getVariableIndex(vars, varCount, c)];
  }
}

void removeFromArray(char * array, int index, int arrayLength) {
  for (int i = index; i < arrayLength - 1; i++)
    array[i] = array[i + 1];
}

void resolveParenthesis(char * expr, Variable * vars, int varCount, bool * values) {
  int size = sizeof(expr) / sizeof(expr[0]);

  for (int i = 0; i < size; i++) {
    if (expr[i] == '~') {
      char affectedChar = expr[i + 1];
      bool value = getVariableValueFromName(affectedChar, vars, varCount, values);

      expr[i] = !value ? 'V' : 'F';

      removeFromArray(expr, i + 1, size);

      resolveParenthesis(expr, vars, varCount, values);
      break;
    }
  }

  for (int i = 0; i < size; i++) {
    if (expr[i] == '>' || expr[i] == '^' || expr[i] == 'v' || expr[i] == '=') {
      bool lastValue = getVariableValueFromName(expr[i - 1], vars, varCount, values);
      bool nextValue = getVariableValueFromName(expr[i + 1], vars, varCount, values);
      bool result;

      if (expr[i] == '>') result = !lastValue | nextValue;
      else if (expr[i] == '^') result = lastValue & nextValue;
      else if (expr[i] == 'v') result = lastValue | nextValue;
      else if (expr[i] == '=') result = lastValue == nextValue;

      expr[i - 1] = result ? 'V' : 'F';
      removeFromArray(expr, i, size);
      removeFromArray(expr, i, size);

      resolveParenthesis(expr, vars, varCount, values);
      break;
    }
  }
}

void resolveExpression(char expr[], Variable * vars, int varCount, bool * values) {
  int size = strlen(expr);

  int lastOpenParenInd = -1;
  for (int i = 0; i < size; i++) {
    if (expr[i] == '(') {
      lastOpenParenInd = i;
    } else if (expr[i] == ')') {
      char localExpression[i - lastOpenParenInd];

      strncpy(localExpression, expr + lastOpenParenInd + 1, i - lastOpenParenInd - 1);

      resolveParenthesis(localExpression, vars, varCount, values);
      expr[lastOpenParenInd] = localExpression[0];

      for (int j = i; j >= lastOpenParenInd + 1; j--) {
        removeFromArray(expr, j, size + 1);
      }

      resolveExpression(expr, vars, varCount, values);
      break;
    }
  }

  resolveParenthesis(expr, vars, varCount, values);
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

  Variable variables[8];
  int varCount = 0;

  for (int i = 0; i < exprSize; i++) {
    if (expr[i] >= 'a' && expr[i] <= 'z' && expr[i] != 'v') {
      if (!contains(variables, varCount, expr[i])) {
        Variable
        var = {
          expr[i],
          varCount
        };
        variables[varCount] =
          var;
        varCount++;
      }
    }
  }

  for (int i = 0; i < varCount; i++) {
    printf("%c  ", variables[i].name);
  }

  printf("|  %s\n", expr);
  printf("-----------------------------\n");

  unsigned char count = 0;

  for (int i = 0; i < pow(2, varCount); i++) {
    bool bits[varCount];

    for (int j = 0; j < varCount; j++) {
      bits[j] = (count >> j) & 0x01;
      printf("%c  ", bits[j] ? 'V' : 'F');
    }

    char exprCopy[exprSize];
    strcpy(exprCopy, expr);
    resolveExpression(exprCopy, variables, varCount, bits);

    printf("|  %c\n", exprCopy[0]);

    count++;
  }
}
