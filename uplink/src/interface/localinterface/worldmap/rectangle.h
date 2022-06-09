#if !defined included_rectangle_h
#define included_rectangle_h

//! Object for rectangles on world map
class MapRectangle {
public:

    /**
     * Top left X coordinate of rectangle
     */
    int x1;

    /**
     * Top left Y coordinate of rectangle
     */
    int y1;

    /**
     * The width of the rectangle
     */
    int width;

    /**
     * The height of the rectangle
     */
    int height;
    
public:

    /**
     * Initializes a rectangle
     * @param x Top left X coordinate
     * @param y Top left Y coordinate
     * @param width Width of rectangle
     * @param height Height of rectangle
     */
    MapRectangle(int x, int y, int width, int height)
	: x1(x), y1(y), width(width), height(height)
	{
	}

	/**
	 * Gets bottom right X coordinate of rectangle
	 * @return
	 */
    int x2() const { return x1 + width - 1; }

    /**
     * Gets bottom right Y coordinate of rectangle
     * @return
     */
    int y2() const { return y1 + height - 1; }

    /**
     * Gets the smaller of two numbers
     * @param a First number
     * @param b Second number
     * @return The smaller number
     */
    static int Min( int a, int b) {
	if (a < b) return a; 
	return b;
    }

    /**
     * Gets the larger of two numbers
     * @param a First number
     * @param b Second number
     * @return The smaller number
     */
    static int Max( int a, int b) {
	if (a > b) return a;
	return b;
    }

    /**
     * Get rectangle of intersection between two rectangles
     * @param o Rectangle to check for intersection with
     * @return A rectangle that matches position and size of intersection
     */
    MapRectangle intersection(const MapRectangle &o) const
	{
	    int newX1 = Max(x1, o.x1);
	    int newY1 = Max(y1, o.y1);
	    int newX2 = Min(o.x2(), x2());
	    int newY2 = Min(o.y2(), y2());
	    
	    return MapRectangle(newX1, newY1, newX2 - newX1 + 1, newY2 - newY1 + 1);
	}

	/**
	 * Check if the rectangle has both a width and height
	 * @return true if both width and height are set
	 */
    bool isNull() const { return (width < 0 || height < 0); };

    /**
     * Checks if two rectangles intersect
     * @param o The rectangle to check for intersection
     * @return true if the rectangles intersect
     */
    bool intersects(const MapRectangle &o) const
	{
	    return !intersection(o).isNull();
	}

	/**
	 * Check if given rectangle is the same as current rectangle
	 * @param o Rectangle to check if same
	 * @return true if rectangles are the same
	 */
    bool operator == (const MapRectangle &o) const 
	{
	    return (x1 == o.x1) && (y1 == o.y1) && (width == o.width) && (height == o.height);
	}

    /**
     * Check if given rectangle is fully contained by this rectangle
     * @param o Rectangle to check if fully contained
     * @return true if fully contained
     */
    bool contains(const MapRectangle &o) const
	{
	    return intersection(o) == o;
	}
    
};

// inline ostream &operator << (ostream &o, const MapRectangle& r)
// {
//     o << "MapRectangle(x1=" << r.x1 << ", y1=" << r.y1 << ", width=" << r.width << ", height=" << r.height << ")";
//     return o;
// }



#endif
