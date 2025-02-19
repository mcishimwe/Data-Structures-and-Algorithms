// Datastructures.cc



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
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::get_affiliation_count()
{
    return affiliations_vector.size();
}

void Datastructures::clear_all()
{
    affiliations_vector.clear();
    affiliation_publication.clear();
    affiliation_map.clear();
    publications_vector.clear();
    publications_vector.clear();
    affiliations_distance.clear();
    affiliations_set.clear();
    coord_to_affiliation.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    return affiliations_vector;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    auto it = affiliation_publication.find(id);

    if(it == affiliation_publication.end()){
        affiliation_publication[id];
        affiliations_vector.push_back(id);


        Affiliation new_affiliation;
        new_affiliation.id = id;
        new_affiliation.name = name;
        new_affiliation.xy = xy;
        affiliation_map.insert({id,new_affiliation});
        affiliations_distance.insert(new_affiliation);
        coord_to_affiliation[xy] = id;
        affiliations_set.insert(new_affiliation);
        return true;
    }
    return false;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
   auto it = affiliation_map.find(id);
   if(it != affiliation_map.end()){
       return it->second.name;
   }
   else{
       return NO_NAME;
   }
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto it = affiliation_map.find(id);
    if(it != affiliation_map.end()){
        return it->second.xy;
    }
    else{
        return NO_COORD;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    static bool is_ordered = false;

    if(!is_ordered){
        affiliations_set = std::set<Affiliation,AffiliationNameComparator>(affiliations_set.begin(),affiliations_set.end());
        is_ordered = true;
    }


    std::vector<AffiliationID> sorted_ids;
    for (const auto& affiliation : affiliations_set) {
        sorted_ids.push_back(affiliation.id);
    }

    return sorted_ids;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    static bool is_ordered = false;

    if(!is_ordered){
        affiliations_distance = std::set<Affiliation,DistanceComparator>(affiliations_distance.begin(),affiliations_distance.end());
        is_ordered = true;
    }

    std::vector<AffiliationID> sorted_affiliationids;
    for (const auto& affiliation : affiliations_distance) {
        sorted_affiliationids.push_back(affiliation.id);
    }
    return sorted_affiliationids;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{
    auto it = coord_to_affiliation.find(xy);
    if (it != coord_to_affiliation.end()) {
        return it->second;
    }
    else {
        return NO_AFFILIATION;
    }
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto it = affiliation_map.find(id);

        if (it != affiliation_map.end()) {

            Affiliation& affiliation = it->second;

            affiliations_distance.erase(affiliation);


            Coord oldcoord = affiliation.xy;
            affiliation.xy = newcoord;

            affiliations_distance.insert(affiliation);




            coord_to_affiliation.erase(oldcoord);
            coord_to_affiliation[newcoord] = id;

            return true;
        }
        else {

            return false;
        }
}

bool Datastructures::add_publication(PublicationID id, const std::string& title, Year year, const std::vector<AffiliationID>& affiliations)
{

    if (publication_map.find(id) != publication_map.end()) {
        return false;
    }


    Publication new_publication;
    new_publication.id = id;
    new_publication.title = title;
    new_publication.publication_year = year;
    new_publication.parent = nullptr;


    for (const AffiliationID& affiliation_id : affiliations) {
        new_publication.publication_affiliation.insert(affiliation_id);
        affiliation_publication[affiliation_id].insert(id);
    }


    publications_vector.push_back(id);
    publication_map[id] = new_publication;

    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    return publications_vector;
}

Name Datastructures::get_publication_name(PublicationID id)
{

    auto publication_it = publication_map.find(id);
    if (publication_it != publication_map.end()) {

        return publication_it->second.title;
    }
    else {

        return NO_NAME;
    }
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto publication_it = publication_map.find(id);
    if(publication_it != publication_map.end()){
        return publication_it ->second.publication_year;
    }
    else{
        return NO_YEAR;
    }
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto it = publication_map.find(id);

    if (it != publication_map.end()) {

        const Publication& publication = it->second;


        std::vector<AffiliationID> result_vector(publication.publication_affiliation.begin(), publication.publication_affiliation.end());

        return result_vector;
    } else {

        return {NO_AFFILIATION};
    }
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto it_child = publication_map.find(id);
    auto it_parent = publication_map.find(parentid);


    if (it_child != publication_map.end() && it_parent != publication_map.end()) {

        it_child->second.parent = &(it_parent->second);
        it_parent->second.references.insert(&(it_child->second));
        return true;
    } else {

        return false;
    }
}

std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto it = publication_map.find(id);

    if (it != publication_map.end()) {

        const Publication& publication = it->second;


        std::vector<PublicationID> direct_references(publication.references.size());
        std::transform(publication.references.begin(), publication.references.end(), direct_references.begin(),
                       [](const Publication* ref) { return ref->id; });

        return direct_references;
    } else {

        return {NO_PUBLICATION};
    }
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{

    auto publication_it = publication_map.find(publicationid);
    if (publication_it == publication_map.end()) {

        return false;
    }


    auto affiliation_it = affiliation_map.find(affiliationid);
    if (affiliation_it == affiliation_map.end()) {

        return false;
    }


    Publication& publication = publication_it->second;
    publication.publication_affiliation.insert(affiliationid);


    affiliation_publication[affiliationid].insert(publicationid);

    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{

    if (affiliation_publication.find(id) == affiliation_publication.end()) {

        return {NO_PUBLICATION};
    }


    const std::unordered_set<PublicationID>& publications = affiliation_publication[id];


    std::vector<PublicationID> publication_vector(publications.begin(), publications.end());

    return publication_vector;
}

PublicationID Datastructures::get_parent(PublicationID id)
{

    auto publication_it = publication_map.find(id);
    if (publication_it == publication_map.end()) {

        return NO_PUBLICATION;
    }


    const Publication* parent_publication = publication_it->second.parent;

    if (parent_publication) {

        return parent_publication->id;
    } else {

        return NO_PUBLICATION;
    }
}

std::vector<std::pair<Year, PublicationID>> Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{

    auto affiliation_it = affiliation_map.find(affiliationid);
    if (affiliation_it == affiliation_map.end()) {

        return {{NO_YEAR, NO_PUBLICATION}};
    }


    const std::unordered_set<PublicationID>& publications = affiliation_publication[affiliationid];


    std::vector<std::pair<Year, PublicationID>> result;


    for (PublicationID publication_id : publications) {

        auto publication_it = publication_map.find(publication_id);
        if (publication_it != publication_map.end()) {
            const Publication& publication = publication_it->second;


            if (publication.publication_year >= year) {

                result.push_back({publication.publication_year, publication.id});
            }
        }
    }


    std::sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) {
            return a.first < b.first;
        } else {
            return a.second < b.second;
        }
    });

    return result;
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id) {

    auto publication_it = publication_map.find(id);
    if (publication_it == publication_map.end()) {

        return {NO_PUBLICATION};
    }

    std::vector<PublicationID> referenced_chain;


    std::function<void(Publication*)> traverse_chain = [&](Publication* pub) {
        if (pub != nullptr) {

            referenced_chain.push_back(pub->id);


            traverse_chain(pub->parent);
        }
    };


    traverse_chain(publication_it->second.parent);

    return referenced_chain;
}

//Optional functions
std::vector<PublicationID> Datastructures::get_all_references(PublicationID id)
{

    auto publication_it = publication_map.find(id);
    if (publication_it == publication_map.end()) {

        return {NO_PUBLICATION};
    }


    std::vector<PublicationID> all_references;


    std::queue<Publication*> bfs_queue;


    std::unordered_set<PublicationID> visited;


    bfs_queue.push(&(publication_it->second));
    visited.insert(id);


    while (!bfs_queue.empty()) {
        Publication* current_publication = bfs_queue.front();
        bfs_queue.pop();

        if (current_publication->id != id) {
            all_references.push_back(current_publication->id);
        }


        for (Publication* reference : current_publication->references) {
            if (visited.find(reference->id) == visited.end()) {
                bfs_queue.push(reference);
                visited.insert(reference->id);
            }
        }
    }

    return all_references;
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord xy)
{
    if(affiliations_set.empty()){
        return {};
    }

    std::vector<AffiliationID> closest_affiliations;

    auto comparator = [&](const Affiliation& a, const Affiliation& b){
        return calculate_distance(a.xy,xy) < calculate_distance(b.xy,xy);
    };

    std::set<Affiliation, decltype(comparator)> close_affiliations(comparator);

    for(const auto& affiliation: affiliations_set){
        close_affiliations.insert(affiliation);

        if(close_affiliations.size()> 3){
            close_affiliations.erase(std::prev(close_affiliations.end()));
        }
    }

    for(const auto& affiliation: close_affiliations){
        closest_affiliations.push_back(affiliation.id);
    }

    return closest_affiliations;
}

bool Datastructures::remove_affiliation(AffiliationID id)
{
    auto iter = affiliation_map.find(id);

    if(iter == affiliation_map.end()){
        return false;
    }

    affiliation_map.erase(id);

    affiliation_publication.erase(id);

    affiliations_vector.erase(std::remove(affiliations_vector.begin(),affiliations_vector.end(),id),affiliations_vector.end());


    for(const PublicationID& publication_id : publications_vector){
        auto publication_iter = publication_map.find(publication_id);

        if(publication_iter != publication_map.end()){
            Publication& publication = publication_iter->second;

            publication.publication_affiliation.erase(id);
        }
    }

    affiliations_set.erase(std::find_if(affiliations_set.begin(),affiliations_set.end(),[id](const Affiliation& a){
        return a.id == id;}));

    affiliations_distance.erase(std::find_if(affiliations_distance.begin(),affiliations_distance.end(),[id](const Affiliation& a){
        return a.id == id;}));

    return true;
}

PublicationID Datastructures::get_closest_common_parent(PublicationID id1, PublicationID id2)
{
    auto iter1 = publication_map.find(id1);
    auto iter2 = publication_map.find(id2);

    if(iter1 == publication_map.end()|| iter2 == publication_map.end()){
        return NO_PUBLICATION;
    }

    std::vector<PublicationID> chain1 = get_referenced_by_chain(id1);
    std::vector<PublicationID> chain2 = get_referenced_by_chain(id2);

    PublicationID common_parent = find_closest_common_parent(chain1,chain2);

    return common_parent;
}

bool Datastructures::remove_publication(PublicationID publicationid)
{
    auto iter = publication_map.find(publicationid);

    if(iter == publication_map.end()){
        return false;
    }



    const Publication& publication_toremove = iter->second;

    for(auto&entry : publication_map){
        auto& other_publication = entry.second;
        if(other_publication.parent == &publication_toremove){
            other_publication.parent = nullptr;
        }
    }

    for(auto& affiliation_entry : affiliation_publication){
        auto& publication_set = affiliation_entry.second;
        publication_set.erase(publicationid);
    }

    publication_map.erase(publicationid);
    publications_vector.erase(std::remove(publications_vector.begin(),publications_vector.end(),publicationid),publications_vector.end());

    return true;
}


