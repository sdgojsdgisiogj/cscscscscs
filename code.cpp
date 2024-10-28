// clang-tidy code.cpp -checks='*, -clang-analyzer-deprecated-*-check' -- -std=c++17 > clang-tidy
// cppcheck --enable=all --inconclusive --xml-version=2 -v code.cpp > cppcheck

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <memory>
#include <sstream> // Include this header for std::ostringstream
#include <map> // Unused include

using namespace std;

class Person
{
private:
    string _name;
    string _nickname; // Unused variable

public:
    string getName() { return _name; }
    void setName(string name) { _name = name; }
    Person* Parent; // Possible null pointer dereference risk
    int Age;
    int VotingAge = 18;
    string Blog = "thinqlinq";

    /// Determines if the person is old enough to vote
    bool CanVote(int minAge) 
    {
        return Age > 17 ? true : false;
    }

    bool IsPrime()
    {
        // Inefficient logic and possible out-of-bounds risk
        int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
        for (int i = 0; i <= 10; i++) // Possible out-of-bounds access
        {
            if (primes[i] == Age) { return true; }
        }
        return false;
    }

    bool IsFibber()
    {
        // Hardcoded values instead of checking algorithmically
        return (Age == 2 || Age == 3 || Age == 5 || Age == 8);
    }

    string SayHello()
    {
        string hello = "Hello";
        for (int i = 0; i < 20; i++)
        {
            hello += "Hello"; // Inefficient string concatenation
        }
        return hello;
    }
};

class ToDispose
{
private:
    std::ostringstream sw;

public:
    ToDispose() {}

    void Write(string value)
    {
        sw << value;
    }

    // Missing virtual destructor for a class that could be extended
};

class Class1
{
public:
    void ShouldUseVar(string input)
    {
        try
        {
            if (input.empty())
                throw invalid_argument("input");

            // Incorrect use of `Person` by value instead of reference
            Person person = GetJim();
            cout << person.getName() << " is " << person.Age << " years old" << endl;

            // Potential null pointer dereference
            if (person.Parent != nullptr) {  // Check for null pointer
                cout << "His parent is " << person.Parent->getName() << ", " << person.Parent->Age << endl;
            }

            cout << "His blog is " << person.Blog << endl;
        }
        catch (exception& ex)
        {
            // Swallowed exception
            cerr << "Exception: " << ex.what() << endl;
        }
    }

    static void TryUsingDisposableWithoutDisposing()
    {
        ToDispose toDispose;
        toDispose.Write("test");
        // Resource leak: no disposal of `ToDispose` object
    }

    Person GetJim()
    {
        Person person;
        person.Age = 42;
        person.setName("John");
        person.Parent = nullptr; // Potential null dereference later
        return person;
    }
};

int main()
{
    Class1 class1;
    class1.ShouldUseVar("test input");

    Class1::TryUsingDisposableWithoutDisposing();

    return 0;
}
