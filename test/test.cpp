#include "CppUnitTest.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iterator>
#include <numeric>
#include <random>
#include <cmath>

using namespace std::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace all_tests
{
    TEST_CLASS(test_v10)
    {
    public:
        TEST_METHOD(test_01)
        {
            std::stringstream ss("14 -78 22");
            std::vector<int> v;
            std::copy(std::istream_iterator<int>(ss), std::istream_iterator<int>(), std::back_inserter(v));

            Assert::AreEqual(3ull, v.size());
            Assert::AreEqual(14, v[0]);
            Assert::AreEqual(-78, v[1]);
            Assert::AreEqual(22, v[2]);
        }
        TEST_METHOD(test_02a)
        {
            std::vector<int> v(10);
            std::iota(v.begin(), v.end(), 1);

            Assert::AreEqual(10ull, v.size());
            Assert::IsTrue(std::is_sorted(v.cbegin(), v.cend()));
            Assert::AreEqual(1, v[0]);
            Assert::AreEqual(10, v[9]);
        }
        TEST_METHOD(test_02b)
        {
            std::vector<int> v(10);
            int n = -1;
            std::generate(v.begin(), v.end(), [&n]() mutable { return n += 2; });

            Assert::IsTrue(std::is_sorted(v.cbegin(), v.cend()));
            Assert::IsTrue(v.cend() == std::adjacent_find(v.cbegin(), v.cend(), [](int a, int b) { return b - a != 2; }));
            Assert::AreEqual(1, v[0]);
            Assert::AreEqual(19, v[9]);
        }

        TEST_METHOD(test_03a)
        {
            std::vector<int> v = { 1, 5, 10 };
            std::transform(v.begin(), v.end(), v.begin(), [](int x) { return static_cast<int>(std::pow(x, 3)); });

            Assert::AreEqual(3ull, v.size());
            Assert::AreEqual(1, v[0]);
            Assert::AreEqual(125, v[1]);
            Assert::AreEqual(1000, v[2]);
        }
        TEST_METHOD(test_03b)
        {
            int x[] = { 3, 5, 10 };
            std::vector<int> y = { 4, 12, 10 };
            std::vector<double> d;

            std::transform(std::begin(x), std::end(x), y.begin(), std::back_inserter(d), [](int a, int b) {
                return std::sqrt(a * a + b * b);
                });

            Assert::AreEqual(3ull, d.size());
            Assert::AreEqual(5., d[0]);
            Assert::AreEqual(13., d[1]);
            Assert::AreEqual(std::sqrt(200), d[2]);
        }
        TEST_METHOD(test_04a)
        {
            std::stringstream ss("1.5 2.5 3.5");
            double res = std::accumulate(std::istream_iterator<double>(ss), std::istream_iterator<double>(), 0.0);

            Assert::AreEqual(7.5, res);
        }
        TEST_METHOD(test_04b)
        {
            std::vector<std::string> v{ "V", "S", "I", "T", "E", "!" };
            auto res = std::accumulate(v.begin(), v.end(), "GO "s);

            Assert::AreEqual("GO VSITE!"s, res);
        }
        TEST_METHOD(test_04c)
        {
            struct person { std::string name; int age; };
            std::vector<person> v{ {"Pero", 33}, {"Iva", 25} };
            int total_age = std::accumulate(v.begin(), v.end(), 0, [](int sum, const person& p) {
                return sum + p.age;
                });

            Assert::AreEqual(58, total_age);
        }

        TEST_METHOD(test_05a)
        {
            std::vector<int> v{ -5, 8, 11, 0, -9, 77, -4 };
            auto number_of_negative = std::count_if(v.begin(), v.end(), [](int x) { return x < 0; });

            Assert::AreEqual(3ll, number_of_negative);
        }
        TEST_METHOD(test_05b)
        {
            std::vector<double> v{ 1.5, 8, -11.23, 0, 1e10, 1e10, 1e10, 0, 99 };
            auto number_of_invalid = std::count(v.begin(), v.end(), 1e10);

            Assert::AreEqual(3ll, number_of_invalid);
        }
        TEST_METHOD(test_05c)
        {
            struct point { int x, y; };
            std::vector<point> v{ {1,1}, {-5,3}, {2,2}, {-7,-6}, {9,-4} };
            auto number_in_first_quadrant = std::count_if(v.begin(), v.end(), [](const point& p) {
                return p.x > 0 && p.y > 0;
                });

            Assert::AreEqual(2ll, number_in_first_quadrant);
        }

        TEST_METHOD(test_06)
        {
            std::vector<int> v{ 33, 16, 24, 41, 25, 19, 9 };
            auto is_prime = [](int n) {
                if (n < 2) return false;
                for (int i = 2; i * i <= n; ++i) {
                    if (n % i == 0) return false;
                }
                return true;
                };
            auto first_prime = *std::find_if(v.begin(), v.end(), is_prime);

            Assert::AreEqual(41, first_prime);
        }
        TEST_METHOD(test_07a)
        {
            std::vector<double> v{ 1e10, 8, -11.23, 0, 1e10, 1e10, 1e10, 0, 99 };
            std::replace(v.begin(), v.end(), 1e10, -1.);

            Assert::AreEqual(-1., v[0]);
            Assert::AreEqual(-1., v[4]);
            Assert::AreEqual(-1., v[6]);
        }
        TEST_METHOD(test_07b)
        {
            std::string s("neisporuka");
            std::replace_if(s.begin(), s.end(), [](char c) {
                return std::string("aeiou").find(c) != std::string::npos;
                }, 'x');

            Assert::AreEqual("nxxspxrxkx"s, s);
        }
        TEST_METHOD(test_08a)
        {
            std::vector<double> v{ 1e10, 8, -11.23, 0, 1e10, 1e10, 1e10, 0, 99 };
            v.erase(std::remove(v.begin(), v.end(), 1e10), v.end());

            Assert::AreEqual(5ull, v.size());
            Assert::AreEqual(8., v[0]);
            Assert::AreEqual(99., v[4]);
        }

        TEST_METHOD(test_08b)
        {
            std::string s("poliuretan");
            s.erase(std::remove_if(s.begin(), s.end(), [](char c) {
                return std::string("aeiou").find(c) != std::string::npos;
                }), s.end());

            Assert::AreEqual("plrtn"s, s);
        }
        TEST_METHOD(test_09)
        {
            struct exam { std::string name; int points, grade; };
            std::vector<exam> v{ {"Pero", 55, 2}, {"Iva", 93, 5}, {"Marko", 89, 5} };
            std::sort(v.begin(), v.end(), [](const exam& a, const exam& b) {
                return a.grade > b.grade || (a.grade == b.grade && a.points > b.points);
                });

            Assert::AreEqual("Iva"s, v[0].name);
            Assert::AreEqual("Marko"s, v[1].name);
            Assert::AreEqual("Pero"s, v[2].name);
        }
        TEST_METHOD(test_10)
        {
            std::vector<double> v(2e7);
            std::generate(v.begin(), v.begin() + v.size() / 2, []() { return rand() % 1000; });
            std::generate(v.begin() + v.size() / 2, v.end(), []() { return 1001 + rand() % 1000; });
            v.push_back(1000); // to be median

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(v.begin(), v.end(), g);

            std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());

            Assert::AreEqual(1000., v[v.size() / 2]); // median value
        }
        TEST_METHOD(test_11)
        {
            std::vector<double> v{ 11, 0.5, -97.23, -23.11, 48.78, 22.96, -77 };
            auto [min_it, max_it] = std::minmax_element(v.begin(), v.end());

            Assert::AreEqual(-97.23, *min_it);
            Assert::AreEqual(48.78, *max_it);
        }
        TEST_METHOD(test_12)
        {
            std::vector<int> atp_points{ 8445, 7480, 6220, 5300, 5285 };
            std::vector<int> differences(atp_points.size());
            std::adjacent_difference(atp_points.begin(), atp_points.end(), differences.begin());
            differences.erase(differences.begin());
            auto smallest_difference = *std::min_element(differences.begin(), differences.end(), [](int a, int b) {
                return std::abs(a) < std::abs(b);
                });
            Assert::AreEqual(15, std::abs(smallest_difference));
        }
    };
}

