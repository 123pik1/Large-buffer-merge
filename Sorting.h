class Tape;
class Number;

int countEmpty(Tape **tapes);
int findNonEmpty(Tape **tapes);
int findEmpty(Tape **tapes);
int countNonEmpty(Tape **tapes);

int findMinimumAmongActive(Tape **tapes, int idEmpty, bool *tapeHasData);
void mergeOneRun(Tape **tapes, int idEmpty);
void merging(Tape **tapes, int idEmpty);
int sort(Tape **tapes);
