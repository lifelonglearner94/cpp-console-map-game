// Item-foundation unit tests for ticket T2a (p2game portfolio project).
//
// Verifies the Aufgabe 1c item foundation at its public seam:
//   * p2game::ItemBase     -- the abstract base class of all items
//     (FR-14) with the weight/name/description/value attributes
//     (FR-15..FR-18) and a virtual describe() for console output;
//   * p2game::GoldCoin     -- the first concrete ItemBase child (FR-19);
//   * p2game::ItemFactory  -- the seeded, deterministic item catalog
//     (ADR 0006).
//
// SEAM PINNED BY THIS TEST (the implementation step must match it):
//
//   class ItemBase {
//   public:
//       virtual ~ItemBase() = default;
//       ItemBase(std::string name, std::string description,
//                double weight, double value);
//           // throws std::invalid_argument when weight <= 0 (FR-15)
//       const std::string& name() const;
//       const std::string& description() const;
//       double weight() const;
//       double value() const;
//       virtual std::string describe() const = 0;
//           // ItemBase is abstract through this pure virtual (FR-14);
//           // the override returns non-empty text containing name().
//   };
//
//   class GoldCoin : public ItemBase {
//   public:
//       GoldCoin();  // canonical coin (valid name/description/weight/value)
//       GoldCoin(std::string name, std::string description,
//                double weight, double value);
//       std::string describe() const override;
//   };
//
//   class ItemFactory {
//   public:
//       ItemFactory();  // owns / builds the catalog
//       std::unique_ptr<ItemBase> create(std::mt19937& rng);
//   };
//
// Tile-chest kind, Map item ownership and MapGenerator placement are
// LATER tickets (T2b/T2c) and are intentionally NOT tested here.
//
// Hand-rolled assert-style main: the project uses no third-party
// libraries (GR-4).

// ItemFactory.h comes first: its header must be self-contained (it pulls
// in ItemBase.h, <memory> and <random> itself).
#include "ItemFactory.h"

#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "GoldCoin.h"
#include "ItemBase.h"

namespace {

int g_failure_count = 0;

void expect_true(bool condition, const char* description) {
    if (condition) {
        std::cout << "PASS: " << description << '\n';
    } else {
        std::cerr << "FAIL: " << description << '\n';
        ++g_failure_count;
    }
}

// A test-local concrete ItemBase child. It exercises the ItemBase
// constructor seam directly and overrides describe() with a distinctive
// marker so virtual dispatch can be observed through an ItemBase
// reference.
class TestItem : public p2game::ItemBase {
public:
    TestItem(std::string name, std::string description, double weight, double value)
        : p2game::ItemBase(std::move(name), std::move(description), weight, value) {}

    std::string describe() const override {
        return "TESTITEM:" + name() + " (" + description() + ")";
    }
};

// FR-15 helper: true iff the construction performed by `construct`
// throws exactly std::invalid_argument. Any other exception (or none)
// is a failure of the FR-15 contract.
template <typename Fn>
bool throws_invalid_argument(Fn&& construct) {
    try {
        construct();
    } catch (const std::invalid_argument&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

// A stable, textual fingerprint of a seeded factory run, so two runs can
// be compared attribute by attribute (name, description, weight, value).
std::vector<std::string> run_sequence(std::uint32_t seed, int count) {
    p2game::ItemFactory factory;
    std::mt19937 rng(seed);
    std::vector<std::string> sequence;
    sequence.reserve(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) {
        std::unique_ptr<p2game::ItemBase> item = factory.create(rng);
        if (item) {
            sequence.push_back(item->name() + "|" + item->description() + "|"
                               + std::to_string(item->weight()) + "|"
                               + std::to_string(item->value()));
        } else {
            sequence.push_back("<null>");
        }
    }
    return sequence;
}

}  // namespace

int main() {
    // FR-14 (ADR 0006): ItemBase is the abstract base class of all items.
    // std::is_abstract can only be true when a pure virtual member
    // (describe(), below) is present.
    static_assert(std::is_abstract<p2game::ItemBase>::value,
                  "ItemBase must be abstract (FR-14)");

    // FR-19: GoldCoin is a concrete ItemBase child.
    static_assert(std::is_base_of<p2game::ItemBase, p2game::GoldCoin>::value,
                  "GoldCoin must derive from ItemBase (FR-19)");
    static_assert(!std::is_abstract<p2game::GoldCoin>::value,
                  "GoldCoin must be concrete (FR-19)");
    std::cout << "PASS: ItemBase is abstract and GoldCoin is a concrete child\n";

    // FR-15: weight must be > 0 (constructor-validated); violations throw
    // std::invalid_argument. Exercised through BOTH the test-local
    // derived class and GoldCoin, and for negative weight AND weight == 0.
    expect_true(throws_invalid_argument([] {
                    const TestItem item{"Negative", "bad weight", -1.0, 5.0};
                    (void)item;
                }),
                "derived item with negative weight throws std::invalid_argument (FR-15)");
    expect_true(throws_invalid_argument([] {
                    const TestItem item{"Zero", "zero weight", 0.0, 5.0};
                    (void)item;
                }),
                "derived item with zero weight throws std::invalid_argument (FR-15)");
    expect_true(throws_invalid_argument([] {
                    const p2game::GoldCoin coin{"BadCoin", "negative", -2.5, 3.0};
                    (void)coin;
                }),
                "GoldCoin with negative weight throws std::invalid_argument (FR-15)");
    expect_true(throws_invalid_argument([] {
                    const p2game::GoldCoin coin{"ZeroCoin", "zero", 0.0, 3.0};
                    (void)coin;
                }),
                "GoldCoin with zero weight throws std::invalid_argument (FR-15)");

    // A strictly positive weight must be accepted.
    expect_true(!throws_invalid_argument([] {
                    const TestItem item{"Good", "ok", 0.25, 5.0};
                    (void)item;
                }),
                "a strictly positive weight is accepted (FR-15)");

    // FR-16..FR-18: the accessors return exactly what was passed at
    // construction (via the test-local concrete class).
    const TestItem item{"Amulet", "shines faintly", 1.5, 42.0};
    expect_true(item.name() == "Amulet",
                "name() returns the constructed name (FR-16)");
    expect_true(item.description() == "shines faintly",
                "description() returns the constructed description (FR-17)");
    expect_true(item.weight() == 1.5,
                "weight() returns the constructed weight (FR-15/FR-16)");
    expect_true(item.value() == 42.0,
                "value() returns the constructed value (FR-18)");

    // ADR 0006: describe() exists, is virtual and renders the item for
    // console output. Dispatched through an ItemBase reference it must
    // reach the derived override and yield non-empty text containing the
    // item's name.
    const p2game::ItemBase& base_ref = item;
    const std::string described = base_ref.describe();
    expect_true(!described.empty(),
                "describe() returns non-empty text (ADR 0006)");
    expect_true(described.find("TESTITEM:") != std::string::npos,
                "describe() dispatches virtually through an ItemBase reference (ADR 0006)");
    expect_true(described.find(item.name()) != std::string::npos,
                "describe() renders the item's name (ADR 0006)");

    // FR-19: GoldCoin constructs (default and with explicit attributes)
    // and renders itself.
    const p2game::GoldCoin default_coin{};
    expect_true(!default_coin.name().empty(),
                "default-constructed GoldCoin has a non-empty name (FR-19)");
    expect_true(!default_coin.describe().empty(),
                "default-constructed GoldCoin describe() is non-empty (FR-19)");
    expect_true(default_coin.describe().find(default_coin.name()) != std::string::npos,
                "GoldCoin describe() renders its own name (FR-19)");

    const p2game::GoldCoin explicit_coin{"Ducat", "a shiny coin", 0.03, 7.0};
    expect_true(explicit_coin.name() == "Ducat"
                    && explicit_coin.description() == "a shiny coin"
                    && explicit_coin.weight() == 0.03
                    && explicit_coin.value() == 7.0,
                "GoldCoin attributes round-trip through the accessors (FR-16..FR-19)");

    // ADR 0006: ItemFactory owns the catalog and creates seeded random
    // items. Every produced item must be valid...
    p2game::ItemFactory factory;
    std::mt19937 validity_rng(7);
    bool all_valid = true;
    for (int i = 0; i < 32; ++i) {
        std::unique_ptr<p2game::ItemBase> created = factory.create(validity_rng);
        if (!created || created->weight() <= 0.0 || created->name().empty()
            || created->description().empty()) {
            all_valid = false;
            break;
        }
    }
    expect_true(all_valid,
                "every factory item has weight > 0 and non-empty name/description (ADR 0006)");

    // ...and the same seed must reproduce the same item sequence, while a
    // different seed produces a different one (determinism, ADR 0006).
    const std::vector<std::string> first = run_sequence(12345u, 8);
    const std::vector<std::string> second = run_sequence(12345u, 8);
    const std::vector<std::string> third = run_sequence(54321u, 8);
    expect_true(first == second,
                "same std::mt19937 seed yields the same item sequence (ADR 0006)");
    expect_true(first != third,
                "a different std::mt19937 seed yields a different item sequence (ADR 0006)");

    if (g_failure_count == 0) {
        std::cout << "All item tests passed.\n";
        return 0;
    }
    std::cerr << g_failure_count << " item test(s) failed.\n";
    return 1;
}
