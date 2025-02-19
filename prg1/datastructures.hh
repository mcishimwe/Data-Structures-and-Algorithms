// Datastructures.hh



#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <set>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <cmath>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: One constant time operation which determines the size of the map
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The clear function is linear where n depends on the size of elements in a data structure
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Returning an affiliations vector
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Inserting into the datastructures has log N complexity.
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding operation in a map has logarithmic time complexity.
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding operation in a map has logarithmic time complexity.
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: The sorting and set creation is logn where n is the number of elements the elements inserted to vector.
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: The looping over the set and the insertion into the vector has linear complexity.
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Accessing elements of an ordered map has logarithmic complexity.
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding an element in a map has logn time complexity.
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: Finding an element in a map is logn time complexity.
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Copying elements to a vector is linear.
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding an element in a map is logarithmic.
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding an element in a map is logarithmic.
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Finding an element in a map and copying to a vector.
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding an element in a map is logarithmic.
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Finding an element in a map is logarithmic.
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate: Finding an element in a map is logarithmic.
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate:Adding elements to a vector is linear.
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(logn)
    // Short rationale for estimate:Finding an element in a map is logarithmic.
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate:Finding an element in a map is logarithmic and there is also looping to add elemts to a vector.
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: The depth of the chain is n and affects the performance.
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance: O(B+E)
    // Short rationale for estimate: As the function uses bfs search the time complexity is linear.
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: Insertion into a set is logarithmic and insertion into a vector is linear.
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Erase operation has linear time complexity.
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate:Linear because of the find common parent operation.
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance:O(n)
    // Short rationale for estimate: Linear because of the iterations done.
    bool remove_publication(PublicationID publicationid);


private:
    struct Affiliation{
        AffiliationID id;
        Name name;
        Coord xy;
    };

    struct Publication{
        PublicationID id;
        std::string title;
        Year publication_year;
        Publication* parent;
        std::set<AffiliationID> publication_affiliation;
        std::set<Publication*> references;
    };

    struct DistanceComparator {
        bool operator()(const Affiliation& a, const Affiliation& b) const {
            auto distanceA = std::sqrt(a.xy.x * a.xy.x + a.xy.y * a.xy.y);
            auto distanceB = std::sqrt(b.xy.x * b.xy.x + b.xy.y * b.xy.y);

            if (distanceA != distanceB) {
                return distanceA < distanceB;
            }
            else {
                return a.xy.y < b.xy.y;
            }
        }
    };

    struct AffiliationNameComparator {
        bool operator()(const Affiliation& a, const Affiliation& b) const {

            return a.name < b.name;
        }
    };


    struct AffiliationIDComparator {
        bool operator()(const Affiliation& a, const Affiliation& b) const {
            return a.id < b.id;
        }
    };

    double calculate_distance(const Coord& coord1, const Coord& coord2){
        double dx = coord1.x - coord2.x;
        double dy = coord1.y - coord2.y;

        double result = std::sqrt((dx*dx)+(dy*dy));

        return result;
    }



    PublicationID find_closest_common_parent(const std::vector<PublicationID>& chain1, const std::vector<PublicationID>& chain2){

        auto iter1 = chain1.rbegin();
        auto iter2 = chain2.rbegin();

        while(iter1!=chain1.rend() && iter2 != chain2.rend() && *iter1 == *iter2){
            iter1++;
            iter2++;
        }

        if(iter1 != chain1.rbegin()){
            return *std::prev(iter1);
        }
        else if(iter2 != chain2.rbegin()){
            return *std::prev(iter2);
        }
        else{
            return NO_PUBLICATION;
        }
    }

    std::vector<AffiliationID> affiliations_vector;
    std::set<Affiliation,AffiliationNameComparator> affiliations_set;
    std::set<Affiliation, DistanceComparator> affiliations_distance;
    std::map<AffiliationID, Affiliation> affiliation_map;
    std::vector<PublicationID> publications_vector;
    std::map<PublicationID, Publication> publication_map;
    std::map<Coord, AffiliationID> coord_to_affiliation;
    std::unordered_map<AffiliationID, std::unordered_set<PublicationID>> affiliation_publication;



};

#endif // DATASTRUCTURES_HH
