// Datastructures.hh
//
// Student name:Marius Cedric Ishimwe
// Student email:marius.ishimwe@tuni.fi
// Student number:151104952

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <queue>
#include <unordered_set>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
struct Connection;
// Type for a distance (in arbitrary units)
using Distance = int;

using Path = std::vector<Connection>;
using PathWithDist = std::vector<std::pair<Connection,Distance>>;

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

struct Connection
{
    AffiliationID aff1 = NO_AFFILIATION;
    AffiliationID aff2 = NO_AFFILIATION;
    Weight weight = NO_WEIGHT;
    bool operator==(const Connection& c1) const{
        return (aff1==c1.aff1) && (aff2==c1.aff2) && (weight==c1.weight);
    }
};
const Connection NO_CONNECTION{NO_AFFILIATION,NO_AFFILIATION,NO_WEIGHT};


// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

struct Affiliation {
    AffiliationID id;
    Name name;
    Coord xy;
    double distance;
};

struct Publication {
    PublicationID id;
    Name name;
    Year year;
    std::vector<AffiliationID> by_affiliations;
    PublicationID parent = NO_PUBLICATION;
    std::vector<PublicationID> children = std::vector<PublicationID>();
};

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

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int get_affiliation_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> all_publications();

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_publication_name(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Year get_publication_year(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    PublicationID get_parent(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_publication(PublicationID publicationid);

    // PRG 2 functions:

    // Estimate of performance:O(n)
    // Short rationale for estimate: Hashmap being used for storing affiliationid to publications vector
    std::vector<Connection> get_connected_affiliations(AffiliationID id);

    // Estimate of performance:O(n)
    // Short rationale for estimate: Also hashmap being used for storing affiliation id to publications vector
    std::vector<Connection> get_all_connections();

    // Estimate of performance:O(n^2)
    // Short rationale for estimate: Also hashmap being used for storing affiliation id to publications vector
    Path get_any_path(AffiliationID source, AffiliationID target);

    // PRG2 optional functions

    // Estimate of performance:
    // Short rationale for estimate:
    Path get_path_with_least_affiliations(AffiliationID source, AffiliationID target);

    // Estimate of performance:
    // Short rationale for estimate:
    Path get_path_of_least_friction(AffiliationID source, AffiliationID target);

    // Estimate of performance:
    // Short rationale for estimate:
    PathWithDist get_shortest_path(AffiliationID source, AffiliationID target);


private:

    std::unordered_map<AffiliationID, Affiliation> affiliation_by_ids;


    std::vector<AffiliationID> all_affiliation_ids;
    std::vector<Affiliation> all_affiliations;

    std::vector<Publication> all_Publications;
    std::unordered_map<PublicationID, Publication> publication_by_ids;
    std::unordered_map<AffiliationID, std::vector<PublicationID>> affiliation_publications;

    std::vector<Connection*> all_connections;
    std::unordered_map<AffiliationID, std::vector<Connection*>> connection_by_id;

    void pre_traverse(PublicationID id, std::vector<PublicationID>&result){
        if (publication_by_ids.find(id) != publication_by_ids.end()) {
                result.push_back(id);
                for (PublicationID child : publication_by_ids[id].children) {
                    pre_traverse(child, result);
                }
            }
    }
    void create_connection(Publication pub, AffiliationID aff_to_fix = NO_AFFILIATION){
        if (aff_to_fix == NO_AFFILIATION) {
                for (size_t i = 0; i < pub.by_affiliations.size(); ++i) {

                    for (size_t j = i + 1; j < pub.by_affiliations.size(); ++j) {
                        AffiliationID target;
                        AffiliationID source;
                        if (pub.by_affiliations[i] < pub.by_affiliations[j]) {
                            source = pub.by_affiliations[i];
                            target = pub.by_affiliations[j];
                        }
                        else {
                            source = pub.by_affiliations[j];
                            target = pub.by_affiliations[i];
                        }
                        if (target == source) {
                            break;
                        }
                        bool exist = false;
                        auto it = connection_by_id.find(source);

                        if (it != connection_by_id.end()) {
                            std::vector<Connection*>& connections = it->second;
                            for (Connection* conn : connections) {
                                if (conn->aff1 == source && conn->aff2 == target) {

                                    exist = true;
                                    conn->weight += 1;
                                    break;
                                }
                            }

                            if (!exist) {
                                Connection* connection = new Connection{source, target, 1};
                                Connection* connection_reverse = new Connection{target, source, 1};
                                all_connections.push_back(connection);

                                connection_by_id[source].push_back(connection);
                                connection_by_id[target].push_back(connection_reverse);

                            }

                        }
                        if (exist) {
                            auto it_reverse = connection_by_id.find(target);
                            if (it_reverse != connection_by_id.end()) {
                                std::vector<Connection*>& connections_reverse = it_reverse->second;
                                for (Connection* conn : connections_reverse) {
                                    if (conn->aff1 == target && conn->aff2 == source) {
                                        conn->weight += 1;
                                        break;
                                    }
                                }
                            }

                        }
                    }
                }
            } else {
                for (size_t i = 0; i < pub.by_affiliations.size(); ++i) {
                    AffiliationID target;
                    AffiliationID source;
                    if (pub.by_affiliations[i] < aff_to_fix) {
                        source = pub.by_affiliations[i];
                        target = aff_to_fix;
                    }
                    else {
                        source = aff_to_fix;
                        target = pub.by_affiliations[i];
                    }
                    if (target == source) {
                        break;
                    }
                    bool exist = false;
                    auto it = connection_by_id.find(source);

                    if (it != connection_by_id.end()) {
                        std::vector<Connection*>& connections = it->second;
                        for (Connection* conn : connections) {
                            if (conn->aff1 == source && conn->aff2 == target) {

                                exist = true;
                                conn->weight += 1;
                                break;
                            }
                        }

                        if (!exist) {
                            Connection* connection = new Connection{source, target, 1};
                            Connection* connection_reverse = new Connection{target, source, 1};
                            all_connections.push_back(connection);

                            connection_by_id[source].push_back(connection);
                            connection_by_id[target].push_back(connection_reverse);

                        }

                    }
                    if (exist) {
                        auto it_reverse = connection_by_id.find(target);
                        if (it_reverse != connection_by_id.end()) {
                            std::vector<Connection*>& connections_reverse = it_reverse->second;
                            for (Connection* conn : connections_reverse) {
                                if (conn->aff1 == target && conn->aff2 == source) {
                                    conn->weight += 1;
                                    break;
                                }
                            }
                        }

                    }
                }
            }
    }
    std::vector<PublicationID> get_common_publication(AffiliationID id1, AffiliationID id2){
        std::vector<PublicationID> pub1 = get_publications(id1);
            std::vector<PublicationID> pub2 = get_publications(id2);

            std::vector<PublicationID> commonPublications;
            if (id1 == id2) {
                return commonPublications;
            }
            for (const PublicationID id : pub1) {
                if (std::find(pub2.begin(), pub2.end(), id) != pub2.end()) {
                    commonPublications.push_back(id);
                }
            }

            return commonPublications;
    }
    bool find_path(AffiliationID current, AffiliationID target, std::vector<Connection>& path, std::unordered_map<AffiliationID, bool>& visited){
        if (current == target) {
                return true;
            }

            visited[current] = true;

            for (const Connection* connection : connection_by_id[current]) {


                if (!visited[connection->aff2]) {
                    path.push_back(*connection);

                    if (find_path(connection->aff2, target, path, visited)) {

                        return true;
                    }
                    path.pop_back();
                }
            }

            return false;
    }



};

#endif // DATASTRUCTURES_HH
