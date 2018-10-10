#include <vector>
//#include "line.h"

namespace hough
{

class grid
{
public:
  // corner x (pixels)
  double x;

  // corner y (pixels)
  double y;

  // angle between x axis
  double theta;

  // spacing of the grid lines (pixels)
  double spacing;
};

namespace
{

double thetaDelta(double theta1, double theta2)
{
  if (theta2 < theta1)
  {
    auto tmp = theta2;
    theta2 = theta1;
    theta1 = tmp;
  } // theta1 <= theta2

  return std::min(std::fabs(theta1 - theta2), std::fabs(theta1 + 2 * M_PI - theta2));
}

bool epsilonEqual(const parametrized_line& l1, const parametrized_line& l2, double rhoEpsilon, double thetaEpsilon)
{
  auto td = thetaDelta(l1.theta(), l2.theta());

  return td < thetaEpsilon && std::fabs(l1.rho() - l2.rho()) < rhoEpsilon;
}

parametrized_line mean(const std::vector<parametrized_line>& lines)
{
  if (lines.size() == 0)
  {
    throw new std::exception();
  }

  std::cout << "Line set size " << lines.size() << std::endl;

  auto rhoSum = 0.0;
  auto cosThetaSum = 0.0;
  auto sinThetaSum = 0.0;

  for (auto &line : lines)
  {
    rhoSum += line.rho();
    cosThetaSum += std::cos(line.theta());
    sinThetaSum += std::sin(line.theta());
  }

  return parametrized_line(rhoSum / lines.size(), std::atan2(sinThetaSum, cosThetaSum));
}

parametrized_line getMeanLine(const parametrized_line& line, const std::vector<parametrized_line>& inputlines, double rhoEpsilon, double thetaEpsilon)
{
  std::vector<parametrized_line> ret;
  for (auto &line2 : inputlines)
  {
    if (epsilonEqual(line, line2, rhoEpsilon, thetaEpsilon))
    {
      ret.push_back(line2);
    }
  }

  return mean(ret);
}

std::vector<parametrized_line> linesWithSameTheta(const parametrized_line& l, const std::vector<parametrized_line>& lines)
{
  std::vector<parametrized_line> ret;
  for (auto &line2 : lines)
  {
    if (thetaDelta(l.theta(), line2.theta()) < M_PI * 0.25)
    {
      ret.push_back(line2);
    }
  }
  return ret;
}

void printSortedRhos(std::vector<parametrized_line> lines)
{
    std::sort(lines.begin(), lines.end(), [](parametrized_line a, parametrized_line b) {
        return a.rho() < b.rho();   
    });

    std::cout << "Sorted by rho, grouped by theta" << std::endl;
    parametrized_line* prev = nullptr;
    for (auto & line : lines)
    {
      auto delta = 0.0;
      if (prev != nullptr)
      {
        delta = line.rho() - prev->rho();
      }
      prev = &line;
      std::cout << "Rho: " << line.rho() << " Theta: " << line.theta() << " Delta prev: " << delta << std::endl;
    }
}
}

std::vector<parametrized_line> find_grid(std::vector<parametrized_line> inputlines, double maxRho)
{
  //auto thetaEpsilon = (2 * M_PI) / 50;
  //auto rhoEpsilon = 2 * maxRho / 50;

  auto thetaEpsilon = 0.1;
  auto rhoEpsilon = 0.1;

  auto prunedlines = std::vector<parametrized_line>();

  for (auto &inputline : inputlines)
  {
    bool add = true;
    for (auto &prunedline : prunedlines)
    {
      if (epsilonEqual(inputline, prunedline, rhoEpsilon, thetaEpsilon))
      {
        add = false;
        break;
      }
    }
    if (add)
    {
      prunedlines.push_back(inputline);
    }
  }

  auto finallines = std::vector<parametrized_line>();

  for (auto &prunedline : prunedlines)
  {

    finallines.push_back(getMeanLine(prunedline, inputlines, rhoEpsilon, thetaEpsilon));
    std::cout << "start" << std::endl;
    std::cout << "p theta " << prunedline.theta() << " p rho: " << prunedline.rho() << std::endl;
    std::cout << "f theta " << finallines.back().theta() << " f rho: " << finallines.back().rho() << std::endl;
    std::cout << "end" << std::endl;    
  }

  // we are looking for two thetas with a lot of thetas in, say, 0.25*M_PI radius

  double maxTheta1 = 0.0;
  std::vector<parametrized_line> thetaGroup1;
  for (auto &prunedline1 : finallines)
  {
    auto lines = linesWithSameTheta(prunedline1, finallines);

    if (lines.size() > thetaGroup1.size())
    {
      maxTheta1 = prunedline1.theta();
      thetaGroup1 = lines;
    }
  }


  double maxTheta2 = 0.0;
  std::vector<parametrized_line> thetaGroup2;
  for (auto &prunedline1: finallines)
  {
    if (thetaDelta(prunedline1.theta(), maxTheta1) < M_PI * 0.25)
    {
      continue;
    }

    auto lines = linesWithSameTheta(prunedline1, finallines);

    if (lines.size() > thetaGroup2.size())
    {
      maxTheta2 = prunedline1.theta();
      thetaGroup2 = lines;
    }

  }

  std::cout << "Pruned lines count "  << prunedlines.size() << std::endl;
  std::cout << "Theta1 " << maxTheta1 << " count " << thetaGroup1.size() << " theta2 " << maxTheta2 << " count " << thetaGroup2.size() << std::endl;
  printSortedRhos(thetaGroup1);
  printSortedRhos(thetaGroup2);

  thetaGroup1.insert( thetaGroup1.end(), thetaGroup2.begin(), thetaGroup2.end() );

  return thetaGroup1;

}
}
