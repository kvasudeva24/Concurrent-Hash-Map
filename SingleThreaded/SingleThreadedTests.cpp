#include "SingleThreadedMap.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

namespace {

int g_testNumber = 0;
const int kTotalTests = 20;

void checkTest(const std::string& name, bool condition) {
    ++g_testNumber;

    if (condition) {
        std::cout << "[PASS] Test " << g_testNumber << ": " << name << "\n";
        return;
    }

    std::cout << "[FAIL] Test " << g_testNumber << ": " << name << "\n";
    std::cout << "============================================\n";
    std::cout << "Passed " << (g_testNumber - 1) << "/" << kTotalTests
              << " test cases passed.\n";
    std::cout << "Failed test: " << name << "\n";
    std::cout << "============================================\n";
    std::exit(1);
}

} // namespace

int main() {
    std::cout << "Running HashMap test suite\n";
    std::cout << "============================================\n";

    // 1. Default construction
    HashMap map;
    checkTest("Default construction", map.empty() && map.size() == 0);

    // 2. Insert new key
    auto [inserted1, ptr1] = map.insert("apple", 1);
    checkTest("Insert new key", inserted1 && ptr1 != nullptr && *ptr1 == 1);

    // 3. Size after insert
    checkTest("Size after single insert", map.size() == 1);

    // 4. Insert duplicate key does not overwrite
    auto [inserted2, ptr2] = map.insert("apple", 99);
    checkTest("Insert duplicate key returns false", !inserted2 && *ptr2 == 1);

    // 5. Find existing key
    int* found = map.find("apple");
    checkTest("Find existing key", found != nullptr && *found == 1);

    // 6. Find missing key
    int* missing = map.find("banana");
    checkTest("Find missing key returns nullptr", missing == nullptr);

    // 7. Contains existing key
    checkTest("Contains existing key", map.contains("apple") == true);

    // 8. Contains missing key
    checkTest("Contains missing key", map.contains("banana") == false);

    // 9. operator[] on existing key
    checkTest("operator[] existing key", map["apple"] == 1);

    // 10. operator[] default-inserts missing key
    int& def = map["cherry"];
    checkTest("operator[] default-inserts missing key",
              map.contains("cherry") && def == 0);

    // 11. Erase existing key
    std::size_t erased1 = map.erase("apple");
    checkTest("Erase existing key", erased1 == 1 && !map.contains("apple"));

    // 12. Erase missing key
    std::size_t erased2 = map.erase("doesnotexist");
    checkTest("Erase missing key returns zero", erased2 == 0);

    // 13. Load factor sanity
    checkTest("Load factor within [0, 1]",
              map.load_factor() >= 0.0 && map.load_factor() <= 1.0);

    // 14. Bucket count is nonzero
    checkTest("Bucket count is nonzero", map.bucket_count() > 0);

    // 15. All keys retrievable after forced rehash
    HashMap growMap;
    for (int i = 0; i < 100; ++i) {
        growMap.insert("key" + std::to_string(i), i);
    }
    bool allFound = true;
    for (int i = 0; i < 100; ++i) {
        int* p = growMap.find("key" + std::to_string(i));
        if (p == nullptr || *p != i) {
            allFound = false;
            break;
        }
    }
    checkTest("All keys retrievable after rehash", allFound);

    // 16. Clear empties the map
    growMap.clear();
    checkTest("Clear empties the map", growMap.empty() && growMap.size() == 0);

    // ---- Rule of Five ----

    // 17. Copy constructor produces an independent deep copy
    HashMap original;
    original.insert("x", 10);
    HashMap copyCtor(original);
    copyCtor.insert("y", 20);
    checkTest("Copy constructor creates independent copy",
              original.contains("x") && !original.contains("y") &&
              copyCtor.contains("x") && copyCtor.contains("y"));

    // 18. Copy assignment copies contents independently
    HashMap copyAssigned;
    copyAssigned.insert("preexisting", 1);
    copyAssigned = original;
    checkTest("Copy assignment replaces contents",
              copyAssigned.contains("x") && !copyAssigned.contains("preexisting") &&
              copyAssigned.size() == original.size());

    // 19. Move constructor transfers contents, leaves source valid/empty
    HashMap toMove;
    toMove.insert("moveKey", 42);
    HashMap moveCtor(std::move(toMove));
    checkTest("Move constructor transfers contents",
              moveCtor.contains("moveKey") && moveCtor["moveKey"] == 42 &&
              toMove.empty() && toMove.size() == 0);

    // 20. Move assignment transfers contents, leaves source valid/empty
    HashMap toMoveAssign;
    toMoveAssign.insert("moveAssignKey", 7);
    HashMap moveAssigned;
    moveAssigned.insert("staleKey", 1);
    moveAssigned = std::move(toMoveAssign);
    checkTest("Move assignment transfers contents",
              moveAssigned.contains("moveAssignKey") && !moveAssigned.contains("staleKey") &&
              toMoveAssign.empty() && toMoveAssign.size() == 0);

    std::cout << "============================================\n";
    std::cout << "Passed " << g_testNumber << "/" << kTotalTests
               << " test cases passed.\n";
    std::cout << "All tests passed.\n";

    return 0;
}