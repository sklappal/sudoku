namespace imageprocessing
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

}