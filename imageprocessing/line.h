#include <cstddef> // for size_t
#include <vector>
#include <iostream>
#include <cmath>

namespace hough
{

class line
{
  public:
    line(int x0, int y0, int x1, int y1)
    {
      m_x0 = x0;
      m_y0 = y0;
      m_x1 = x1;
      m_y1 = y1;
    }

    line(const line& l) : line(l.x0(), l.y0(), l.x1(), l.y1())
    {
      
    }

    int x0() const { return m_x0; }
    int y0() const { return m_y0; }
    int x1() const { return m_x1; }
    int y1() const { return m_y1; }

  private:
    double m_x0;
    double m_y0;
    double m_x1;
    double m_y1;

};

class parametrized_line
{
  public:
    parametrized_line(double rho, double theta)
    {
      m_rho = rho;
      m_theta = theta;      
    }

    parametrized_line(const parametrized_line& l) : parametrized_line(l.rho(), l.theta())
    {
      
    }

    double rho() const { return m_rho; }
    double theta() const { return m_theta; }
    
  private:
    double m_rho;
    double m_theta;   

};

class linecontainer
{
  public:
    linecontainer(double maxRho)
    {
      m_lines = std::vector<parametrized_line>();
      m_thetaEpsilon = (2*M_PI) / 50;
      m_rhoEpsilon = 2 * maxRho / 50;
    }

    linecontainer(const linecontainer& linecontainer)
    {
      m_lines = linecontainer.m_lines;
      m_thetaEpsilon = linecontainer.m_thetaEpsilon;
      m_rhoEpsilon = linecontainer.m_rhoEpsilon;
    }

    void add(const parametrized_line& line)
    {
      for(size_t i = 0; i < m_lines.size(); i++)
      {
        auto pline = m_lines[i];
        if (std::fabs(pline.theta() - line.theta()) < m_thetaEpsilon && std::fabs(pline.rho() - line.rho()) < m_rhoEpsilon)
        {
          return;
        }
      }
      m_lines.push_back(line);
    }

    size_t size()
    {
      return m_lines.size();
    }

    parametrized_line at(size_t i)
    {
      return m_lines.at(i);
    }


  private:
    std::vector<parametrized_line> m_lines;
    double m_rhoEpsilon;
    double m_thetaEpsilon;
};

}