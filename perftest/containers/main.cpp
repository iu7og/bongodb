// Copyright 2022 iu7og
#include <QCoreApplication>
#include <chrono> //NOLINT
#include <iostream>
#include <map>

/*
 * gen_random - funtion for generation of randomly fulled string.
 *
 * const int len - size of generated string
 */
std::string gen_random(const int len) {
  static const char alphanum[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
  std::string tmp_s;
  tmp_s.reserve(len);

  for (int i = 0; i < len; ++i) {
    tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return tmp_s;
}

//<< DEBUG TOOL containers
template <template <typename...> typename TMap, typename... MapArgs>
std::ostream &operator<<(std::ostream &out, const TMap<MapArgs...> &m) {
  for (const auto &[k, v] : m)
    out << k << ' ' << v << '\n';
  return out;
}
//<< DEBUG TOOL containers ENDS

//! Experiment constants
#define START_NUMBER_OF_ELEMENTS 1000
#define TERM_FOR_NEW_EXPERIMENT 1000
#define NUMBER_OF_EXPERIMENTS 100
#define REPEATS_OF_EACH_EXPERIMENT 5
//! End of experiment constants

//! Experiment data constants
#define KEY_LENGTH 10
//! End of experiment data constants

int main(int argc, char *argv[]) {
  size_t currentNumberOfElements = START_NUMBER_OF_ELEMENTS;

  std::vector<size_t> summaryTimes(NUMBER_OF_EXPERIMENTS);

  for (int i = 0; i < REPEATS_OF_EACH_EXPERIMENT; i++) {
    std::unordered_map<std::string, int> mapToTest;
    std::vector<std::string> randomStrings;

    for (int j = 0; j < START_NUMBER_OF_ELEMENTS +
                            TERM_FOR_NEW_EXPERIMENT * NUMBER_OF_EXPERIMENTS;
         j++)
      randomStrings.push_back(gen_random(KEY_LENGTH));

    for (int j = 0, currentNumberOfElements = START_NUMBER_OF_ELEMENTS;
         j < NUMBER_OF_EXPERIMENTS;
         j++, currentNumberOfElements += TERM_FOR_NEW_EXPERIMENT) {
      std::chrono::steady_clock::time_point begin =
          std::chrono::steady_clock::now();
      for (int k = currentNumberOfElements - START_NUMBER_OF_ELEMENTS;
           k < currentNumberOfElements; k++) {
        mapToTest[randomStrings[k]] = k;
      }
      std::chrono::steady_clock::time_point end =
          std::chrono::steady_clock::now();

      summaryTimes[j] +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
              .count();
    }
  }

  for (int i = 0; i < NUMBER_OF_EXPERIMENTS; i++) {
    std::cout << "Number of elements: "
              << START_NUMBER_OF_ELEMENTS + i * TERM_FOR_NEW_EXPERIMENT
              << "; Result: " << summaryTimes[i] / REPEATS_OF_EACH_EXPERIMENT
              << std::endl;
  }
}
