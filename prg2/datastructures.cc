// Datastructures.cc
//
// Student name:Marius Cedric Ishimwe
// Student email:marius.ishimwe@tuni.fi
// Student number:151104952

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <queue>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    all_affiliations = std::vector<Affiliation>();
    all_affiliation_ids= std::vector<AffiliationID>();

    affiliation_by_ids = std::unordered_map<AffiliationID, Affiliation>();

    publication_by_ids = std::unordered_map<PublicationID, Publication>();
    affiliation_publications = std::unordered_map<AffiliationID, std::vector<PublicationID>>();
}

Datastructures::~Datastructures()
{
    clear_all();
}

unsigned int Datastructures::get_affiliation_count()
{
    return all_affiliations.size();
}

void Datastructures::clear_all()
{
    all_affiliations.clear();
    all_affiliation_ids.clear();
    affiliation_by_ids.clear();

    publication_by_ids.clear();
    affiliation_publications.clear();

    std::unordered_set<Connection*> connection_set;


        for (Connection* conn_ptr : all_connections) {
            connection_set.insert(conn_ptr);
        }


        for (const auto& kvp : connection_by_id) {
            const std::vector<Connection*>& connections = kvp.second;
            for (Connection* conn_ptr : connections) {
                connection_set.insert(conn_ptr);
            }
        }
        for (Connection* conn_ptr : connection_set) {
            delete conn_ptr;
        }
        connection_set.clear();
        all_connections.clear();
        connection_by_id.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    return all_affiliation_ids;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    double distance = xy.x * xy.x + xy.y * xy.y;
    Affiliation new_affiliation = {id, name, xy, distance};
    affiliation_by_ids.insert({id, new_affiliation});
    all_affiliations.push_back(new_affiliation);
    all_affiliation_ids.push_back(id);
    affiliation_publications[id] = {};

    connection_by_id[id] = std::vector<Connection*>();
    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    if (affiliation_by_ids.find(id) != affiliation_by_ids.end()) {
            return affiliation_by_ids[id].name;
        }
        return NO_NAME;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    if (affiliation_by_ids.find(id) != affiliation_by_ids.end()) {
            return affiliation_by_ids[id].xy;
        }
        return NO_COORD;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    std::vector<AffiliationID> result(all_affiliations.size());


        std::sort(all_affiliations.begin(), all_affiliations.end(),
                  [](const Affiliation &a, const Affiliation &b) {
                      return a.name < b.name;
                  });

        for (std::size_t i = 0; i < all_affiliations.size(); ++i) {
            result[i] = all_affiliations[i].id;
        }
        return result;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{

        std::sort(all_affiliations.begin(), all_affiliations.end(),
                  [](const Affiliation &a, const Affiliation &b) {
                      if (a.distance != b.distance) {
                          return a.distance < b.distance;
                      } else {

                          return a.xy.y < b.xy.y;
                      }
                  });
        std::vector<AffiliationID> result(all_affiliations.size());
        for (std::size_t i = 0; i < all_affiliations.size(); ++i) {
            result[i] = all_affiliations[i].id;
        }

        return result;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    std::sort(all_affiliations.begin(), all_affiliations.end(), [](const Affiliation& a, const Affiliation& b) {
            return a.xy.x < b.xy.x || (a.xy.x == b.xy.x && a.xy.y < b.xy.y);
        });


        auto it = std::lower_bound(all_affiliations.begin(), all_affiliations.end(), xy,
                                   [](const Affiliation& a, const Coord& xy) {
                                       return a.xy.x < xy.x || (a.xy.x == xy.x && a.xy.y < xy.y);
                                   });

        if (it != all_affiliations.end() && it->xy.x == xy.x && it->xy.y == xy.y) {
            return it->id;
        } else {
            return NO_AFFILIATION;
        }
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    double distance = newcoord.x * newcoord.x + newcoord.y * newcoord.y;

        auto it = std::find_if(all_affiliations.begin(), all_affiliations.end(),
                               [id](const Affiliation &affiliation) {
                                   return affiliation.id == id;
                               });

        if (it != all_affiliations.end()) {

            it->xy = newcoord;
            it->distance = distance;
        }
        else {
            return false;
        }

        if (affiliation_by_ids.find(id) != affiliation_by_ids.end()) {
            affiliation_by_ids[id].xy = newcoord;
            affiliation_by_ids[id].distance = distance;
            return true;
        }
        return false;
}

bool Datastructures::add_publication(PublicationID id, const std::string& title, Year year, const std::vector<AffiliationID>& affiliations)
{

    Publication new_publication = {id, title, year, affiliations};
        publication_by_ids.emplace(id, new_publication);
        for (const auto& affid:affiliations) {
            if (affiliation_publications.find(affid) == affiliation_publications.end()) {
                affiliation_publications.insert({affid, std::vector<PublicationID>()});
            }
            affiliation_publications[affid].push_back(id);
        }
        create_connection(new_publication);
        return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> result;
        result.reserve(publication_by_ids.size());

        for (auto it = publication_by_ids.begin(); it != publication_by_ids.end(); ++it) {
            result.push_back(it->first);
        }
        return result;
}

Name Datastructures::get_publication_name(PublicationID id)
{

    if (publication_by_ids.find(id) != publication_by_ids.end()) {
            return publication_by_ids[id].name;
        }
        return NO_NAME;
}

Year Datastructures::get_publication_year(PublicationID id)
{
    if (publication_by_ids.find(id) != publication_by_ids.end()) {
            return publication_by_ids[id].year;
        }
        return NO_YEAR;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    if (publication_by_ids.find(id) != publication_by_ids.end()) {
            return publication_by_ids[id].by_affiliations;
        }
        return {NO_AFFILIATION};
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto referencing = publication_by_ids.find(parentid);
        auto referenced = publication_by_ids.find(id);
        if (referencing != publication_by_ids.end() && referenced != publication_by_ids.end()) {
            referenced->second.parent = parentid;
            referencing->second.children.push_back(id);
            return true;
        }
        return false;
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    if (publication_by_ids.find(id) != publication_by_ids.end()) {
            return publication_by_ids[id].children;
        }
        return {};
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{

    auto pub = publication_by_ids.find(publicationid);
        if (pub != publication_by_ids.end() && affiliation_by_ids.find(affiliationid) != affiliation_by_ids.end()) {
            pub->second.by_affiliations.push_back(affiliationid);
            affiliation_publications[affiliationid].push_back(publicationid);
            create_connection(pub->second, affiliationid);
            return true;
        }
        return false;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{

    if (affiliation_publications.find(id) != affiliation_publications.end()) {
            return affiliation_publications[id];
        }
        return {NO_PUBLICATION};
}

PublicationID Datastructures::get_parent(PublicationID id)
{

    if (publication_by_ids.find(id) != publication_by_ids.end()) {
            return publication_by_ids[id].parent;
        }
        return NO_PUBLICATION;
}

std::vector<std::pair<Year, PublicationID>> Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{

    if (affiliation_publications.find(affiliationid) != affiliation_publications.end()) {
            std::vector<std::pair<Year, PublicationID>> result;
            auto aff = affiliation_publications[affiliationid];
            for (auto pubid : aff) {
                if (publication_by_ids.find(pubid) != publication_by_ids.end()) {
                    if (publication_by_ids[pubid].year >= year) {
                        result.emplace_back(publication_by_ids[pubid].year, pubid);
                    }
                }
            }

            std::sort(result.begin(), result.end(), [](const auto &a, const auto &b) {
                if (a.first == b.first) {
                    return a.second < b.second;
                }
                return a.first < b.first;
            });

            return result;
        }
        return {{NO_YEAR, NO_PUBLICATION}};
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id) {

    if (publication_by_ids.find(id) != publication_by_ids.end()) {
            auto current = publication_by_ids[id];
            std::vector<PublicationID> result;
            while (current.parent != NO_PUBLICATION) {
                result.push_back(current.parent);
                current = publication_by_ids[current.parent];
            }
            return result;
        }
        return {NO_PUBLICATION};
}

//Optional functions
std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{

    if (publication_by_ids.find(id) != publication_by_ids.end()) {
            auto pub = publication_by_ids[id];
            std::vector<PublicationID> result;
            pre_traverse(id, result);
            if (result.size() > 1) {
                result.erase(result.begin());
                return result;
            }
            return {};
        }
        else {
            return {NO_PUBLICATION};
        }
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    std::vector<std::pair<AffiliationID, Affiliation>> sortedPairs(affiliation_by_ids.begin(), affiliation_by_ids.end());


        std::sort(sortedPairs.begin(), sortedPairs.end(), [xy](const auto& a, const auto& b) {

            double distanceA = std::sqrt(std::pow(xy.x - a.second.xy.x, 2) + std::pow(xy.y - a.second.xy.y, 2));
            double distanceB = std::sqrt(std::pow(xy.x - b.second.xy.x, 2) + std::pow(xy.y - b.second.xy.y, 2));
            if (distanceA == distanceB) {
                return a.second.xy.y < b.second.xy.y;
            }
            return distanceA < distanceB;

        });

        std::vector<AffiliationID> result;
        result.reserve(3);
        for (const auto& affiliation : sortedPairs) {
            if (result.size() >= 3) {
                break;
            }
            result.push_back(affiliation.second.id);
        }
        return result;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto affit = std::find_if(all_affiliations.begin(), all_affiliations.end(),
                                  [id](const Affiliation &affiliation) {
                                      return affiliation.id == id;
                                  });

        if (affit != all_affiliations.end()) {
            all_affiliations.erase(affit);
        }


        auto idt = find(all_affiliation_ids.begin(), all_affiliation_ids.end(),
                        id);

        if (idt != all_affiliation_ids.end()) {
            all_affiliation_ids.erase(idt);
        }

        auto it = affiliation_by_ids.find(id);
        if (it != affiliation_by_ids.end()) {

            auto it2 = affiliation_publications.find(id);
            if (it2 != affiliation_publications.end()) {
                for (auto pubId : it2->second) {
                    auto pubIt = publication_by_ids.find(pubId);
                    if (pubIt != publication_by_ids.end()) {

                        pubIt->second.by_affiliations.erase(std::remove(pubIt->second.by_affiliations.begin(),
                                                                        pubIt->second.by_affiliations.end(),
                                                                        id),
                                                            pubIt->second.by_affiliations.end());
                    }
                }
                affiliation_publications.erase(it2);
            }
            affiliation_by_ids.erase(it);
            return true;
        }
        return false;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    std::vector<PublicationID> parents1 = get_referenced_by_chain(id1);
        std::vector<PublicationID> parents2 = get_referenced_by_chain(id2);
        for (const PublicationID& id : parents2) {
            auto publicationIt = std::find_if(parents1.begin(), parents1.end(),
                                              [id](const PublicationID& publicationId) {
                                                  return publicationId == id;
                                              });
            if (publicationIt != parents1.end()) {

                return id;
            }
        }
        return NO_PUBLICATION;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto it = publication_by_ids.find(publicationid);
        if (it != publication_by_ids.end()) {

            if (it->second.parent != NO_PUBLICATION) {
                auto parentIt = publication_by_ids.find(it->second.parent);

                auto removeIt = std::remove(parentIt->second.children.begin(), parentIt->second.children.end(), publicationid);
                parentIt->second.children.erase(removeIt, parentIt->second.children.end());
            }


            if (!it->second.children.empty()) {
                for (PublicationID child : it->second.children) {
                    auto childIt = publication_by_ids.find(child);
                    childIt->second.parent = NO_PUBLICATION;
                }
            }

            if (!it->second.by_affiliations.empty()) {
                for (AffiliationID &affiliation : it->second.by_affiliations) {
                    auto affiliationIt = affiliation_publications.find(affiliation);
                    if (affiliationIt != affiliation_publications.end()) {
                        auto remove_it = std::find(affiliationIt->second.begin(), affiliationIt->second.end(), publicationid);
                        if (remove_it != affiliationIt->second.end()) {
                            affiliationIt->second.erase(remove_it);
                        }
                    }
                }
            }

            publication_by_ids.erase(publicationid);
            return true;
        }
        return false;
}

std::vector<Connection> Datastructures::get_connected_affiliations(AffiliationID id)
{
    if (connection_by_id.find(id) != connection_by_id.end()) {
            std::vector<Connection> result;
            result.reserve(connection_by_id[id].size());
            for (const auto& ptr : connection_by_id[id]) {
                if (ptr != nullptr) {
                    result.emplace_back(*ptr);
                }
            }
            return result;
        }

        return {};
}

std::vector<Connection> Datastructures::get_all_connections()
{
    std::vector<Connection> result;
        result.reserve(all_connections.size());


        for (const auto& ptr : all_connections) {
            if (ptr != nullptr) {
                result.emplace_back(*ptr);
            }
        }
        return result;
}

Path Datastructures::get_any_path(AffiliationID source, AffiliationID target)
{
    Path path;
        std::unordered_map<AffiliationID, bool> visited;

        if (source == target) {
            return path;
        }

        for (Connection* conn : all_connections) {
            visited[conn->aff1] = false;
            visited[conn->aff2] = false;
        }

        if (find_path(source, target, path, visited)) {
            return path;
        }

        return {};
}

Path Datastructures::get_path_with_least_affiliations(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_path_with_least_affiliations()");
}

Path Datastructures::get_path_of_least_friction(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_path_of_least_friction()");
}

PathWithDist Datastructures::get_shortest_path(AffiliationID /*source*/, AffiliationID /*target*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_shortest_path()");
}


