#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();



    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWords.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    /// <summary>
    /// 2ND CYCLE
    /// ASSIGNING GETVALIDWORD VALUE TO A MEMBER VARIABLE TO MINIMIZE PERFORMANCE LOSS WHEN CALLING THE FUNCTION MULTIPLE TIMES
    /// </summary>
    Isograms = GetValidWords(Words);

    SetupGame(); // Setting Up the Game

    //PrintLine(TEXT("%s"), *HiddenWord);

    //PrintLine(TEXT("The number of possible words is: %i"), Words.Num());
    //PrintLine(TEXT("The number of valid words is: %i"), GetValidWords(Words).Num());


}








void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{



    if (bGameOver) {
        ClearScreen();
        SetupGame();
    }
    else {

        ProcessGuess(PlayerInput);

    }




}




/// <summary> 2ND CYCLE
/// The reason why we are place the initialization fuction at the bottom is because 
/// of the fact that in order to read the code of this program, the reader would want to understand how the application functions. 
/// If they want to know HOW the initialization functions work, then they would have a better time coming all the way down and reading those here. 
/// </summary> 2ND CYCLE

void UBullCowCartridge::SetupGame() {





    PrintLine(TEXT("Welcome to Bull Cows Game!"));

    HiddenWord = Isograms[(FMath::RandRange(0, (Isograms.Num() - 1)))];
    Lives = HiddenWord.Len();
    bGameOver = false;

    //  Welcome The Player
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives!"), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));



}









void UBullCowCartridge::EndGame() {

    bGameOver = true;

    PrintLine(TEXT("\nPress enter to play again!"));


}









/// <summary>
/// 2ND CYCLE: USING DIRECT REFERENCES IN THE PARAMETERS INSTEAD OF MAKING A COPY OF THE ARRAY 
/// </summary>


void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    //Checking Player Guess

    if (Guess == HiddenWord) {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len()) {

        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining"), Lives);

        return;
    }


    ///2ND CYCLE
    ///The Checking of IsIsogram function is place below the two above checks to optimize perofrmance.
    /// This function is a lot more demanding in terms of performance and thus is placed at the end 
    /// in case of a worst case scenario. 
    /// 2ND CYCLE


    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("There are no repeating letters in the hidden word, guess again"));
        return;
    }


    PrintLine(TEXT("Lost a life!"));
    --Lives;

    if (Lives <= 0) {

        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The Hidden word was: %s"), *HiddenWord);
        EndGame();
        return;

    }

    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);


    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);


}













/// 2ND CYCLE
/// To make sure that the function that has no business changing any values of the member variables
/// does not do anything suspicious in the background in case of modification, we will make the function
/// const.
/// 2ND CYCLE

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0; i < Word.Len(); i++) {

        for (int32 j = i + 1; j < Word.Len(); j++) {

            if (Word[i] == Word[j]) {
                return false;
            }


        }

    }





    /*for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        PrintLine(TEXT("%c"), Word[Index]);
    }
    */


    return true;
}















TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (int32 a = 0; a < WordList.Num(); a++) {

        if ((WordList[a].Len() >= 4) && (WordList[a].Len() <= 8) && (IsIsogram(WordList[a]))) {

            // We can also use Add() function however, Unreal Documentation recommends Emplace() function to be used for string types. 
            ValidWords.Emplace(WordList[a]);

        }


    }


    return ValidWords;



}









FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const

{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {

        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;          ///2ND CYCLE: SAVING OUT WASTED EFFORT IN THE CODE 
            }
        }

    }

    return Count;
}