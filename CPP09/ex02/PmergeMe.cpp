#include "PmergeMe.hpp"

/*----------------------------------------------------------------------------------*/
/*                                CANONICAL FORM                                    */
/*----------------------------------------------------------------------------------*/

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const std::vector<int> &input) : _input(input) {}

PmergeMe::PmergeMe(const PmergeMe &other) : _input(other._input) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{

    if (this != &other)
        _input = other._input;
    return (*this);
}

PmergeMe::~PmergeMe() {}

/*----------------------------------------------------------------------------------*/
/*                                SORTING  UTILS                                    */
/*----------------------------------------------------------------------------------*/

/*
** L'algoritmo non sposta i valori, ma i loro indici nel container originale.
** Gli indici sono unici anche se ci sono duplicati, quindi a ogni "grande"
** posso ritrovare il suo "piccolo" con partner[grande].
** Il confronto pero' avviene sempre sui valori.
*/
template <typename C>
struct CompareIdx
{
    const C *_val;

    CompareIdx(const C &val) : _val(&val) {}
    bool operator()(size_t a, size_t b) const
    {
        return ((*_val)[a] < (*_val)[b]);
    }
};

/*----------------------------------------------------------------------------------*/
/*                                VECTOR SORTING                                    */
/*----------------------------------------------------------------------------------*/

/*
** Ordine di inserimento dei pending, in indici "b" 1-based.
** b1 e' gia' nella main chain, quindi si parte da b2.
** Jacobsthal: 1 3 5 11 21 ... -> b3 b2 | b5 b4 | b11 ... b6 | ...
*/
static std::vector<size_t> jacobVect(size_t size)
{
    std::vector<size_t> order;
    std::vector<size_t> final;

    if (size < 2)
        return final;
    order.push_back(0);
    order.push_back(1);
    while (order.back() < size)
    {
        size_t n = order[order.size() - 1] + (2 * order[order.size() - 2]);
        order.push_back(n);
    }
    for (size_t i = 2; i < order.size(); i++)
    {
        size_t current = order[i];
        size_t prev = order[i - 1];
        if (current > size)
            current = size;
        while (current > prev)
        {
            final.push_back(current);
            current--;
        }
    }
    return final;
}

static void mainVector(const std::vector<size_t> &bigs, const std::vector<size_t> &partner,
                       std::vector<size_t> &mainchain, std::vector<size_t> &pending)
{
    mainchain.push_back(partner[bigs[0]]);
    for (size_t i = 0; i < bigs.size(); i++)
        mainchain.push_back(bigs[i]);
    for (size_t i = 1; i < bigs.size(); i++)
        pending.push_back(partner[bigs[i]]);
}

static void mergeInsertVect(const std::vector<int> &val, std::vector<size_t> &idx)
{
    if (idx.size() < 2)
        return;

    CompareIdx<std::vector<int> > comp(val);
    std::vector<size_t> bigs;
    std::vector<size_t> partner(val.size());
    bool hasLeft = (idx.size() % 2 != 0);

    // 1. coppie: piccolo -> partner[grande]
    for (size_t i = 0; i + 1 < idx.size(); i += 2)
    {
        size_t small = idx[i];
        size_t big = idx[i + 1];
        if (comp(big, small))
            std::swap(small, big);
        bigs.push_back(big);
        partner[big] = small;
    }

    // 2. ricorsione: ordino i grandi con lo stesso algoritmo
    mergeInsertVect(val, bigs);

    // 3. main chain e pending
    std::vector<size_t> mainchain;
    std::vector<size_t> pending;
    mainVector(bigs, partner, mainchain, pending);
    if (hasLeft)
        pending.push_back(idx[idx.size() - 1]);

    // 4. inserimento dei pending in ordine Jacobsthal
    std::vector<size_t> jacob = jacobVect(pending.size() + 1);
    std::vector<size_t>::iterator bound;
    std::vector<size_t>::iterator it;
    for (size_t i = 0; i < jacob.size(); i++)
    {
        size_t id = jacob[i] - 2;
        // b(k) < a(k): cerco solo prima del suo grande, lo straggler non ha grande
        if (id + 1 < bigs.size())
            bound = std::find(mainchain.begin(), mainchain.end(), bigs[id + 1]);
        else
            bound = mainchain.end();
        it = std::lower_bound(mainchain.begin(), bound, pending[id], comp);
        mainchain.insert(it, pending[id]);
    }
    idx = mainchain;
}

std::vector<int> PmergeMe::processVector()
{
    std::vector<int> val(_input.begin(), _input.end());
    std::vector<size_t> idx;
    std::vector<int> res;

    for (size_t i = 0; i < val.size(); i++)
        idx.push_back(i);
    mergeInsertVect(val, idx);
    for (size_t i = 0; i < idx.size(); i++)
        res.push_back(val[idx[i]]);
    return res;
}

/*----------------------------------------------------------------------------------*/
/*                                DEQUE  SORTING                                    */
/*----------------------------------------------------------------------------------*/

static std::deque<size_t> jacobDeq(size_t size)
{
    std::deque<size_t> order;
    std::deque<size_t> final;

    if (size < 2)
        return final;
    order.push_back(0);
    order.push_back(1);
    while (order.back() < size)
    {
        size_t n = order[order.size() - 1] + (2 * order[order.size() - 2]);
        order.push_back(n);
    }
    for (size_t i = 2; i < order.size(); i++)
    {
        size_t current = order[i];
        size_t prev = order[i - 1];
        if (current > size)
            current = size;
        while (current > prev)
        {
            final.push_back(current);
            current--;
        }
    }
    return final;
}

static void mainDeque(const std::deque<size_t> &bigs, const std::deque<size_t> &partner,
                      std::deque<size_t> &mainchain, std::deque<size_t> &pending)
{
    mainchain.push_back(partner[bigs[0]]);
    for (size_t i = 0; i < bigs.size(); i++)
        mainchain.push_back(bigs[i]);
    for (size_t i = 1; i < bigs.size(); i++)
        pending.push_back(partner[bigs[i]]);
}

static void mergeInsertDeq(const std::deque<int> &val, std::deque<size_t> &idx)
{
    if (idx.size() < 2)
        return;

    CompareIdx<std::deque<int> > comp(val);
    std::deque<size_t> bigs;
    std::deque<size_t> partner(val.size());
    bool hasLeft = (idx.size() % 2 != 0);

    for (size_t i = 0; i + 1 < idx.size(); i += 2)
    {
        size_t small = idx[i];
        size_t big = idx[i + 1];
        if (comp(big, small))
            std::swap(small, big);
        bigs.push_back(big);
        partner[big] = small;
    }

    mergeInsertDeq(val, bigs);

    std::deque<size_t> mainchain;
    std::deque<size_t> pending;
    mainDeque(bigs, partner, mainchain, pending);
    if (hasLeft)
        pending.push_back(idx[idx.size() - 1]);

    std::deque<size_t> jacob = jacobDeq(pending.size() + 1);
    std::deque<size_t>::iterator bound;
    std::deque<size_t>::iterator it;
    for (size_t i = 0; i < jacob.size(); i++)
    {
        size_t id = jacob[i] - 2;
        if (id + 1 < bigs.size())
            bound = std::find(mainchain.begin(), mainchain.end(), bigs[id + 1]);
        else
            bound = mainchain.end();
        it = std::lower_bound(mainchain.begin(), bound, pending[id], comp);
        mainchain.insert(it, pending[id]);
    }
    idx = mainchain;
}

std::deque<int> PmergeMe::processDeque()
{
    std::deque<int> val(_input.begin(), _input.end());
    std::deque<size_t> idx;
    std::deque<int> res;

    for (size_t i = 0; i < val.size(); i++)
        idx.push_back(i);
    mergeInsertDeq(val, idx);
    for (size_t i = 0; i < idx.size(); i++)
        res.push_back(val[idx[i]]);
    return res;
}

/*----------------------------------------------------------------------------------*/
/*                                PROCESS                                           */
/*----------------------------------------------------------------------------------*/

void PmergeMe::process()
{
    clock_t vecTime;
    clock_t deqTime;
    std::vector<int> resVect;
    std::deque<int> resDeq;

    vecTime = clock();
    resVect = processVector();
    vecTime = clock() - vecTime;

    deqTime = clock();
    resDeq = processDeque();
    deqTime = clock() - deqTime;

    std::cout << "After: ";
    printCont(resVect);
    std::cout << "Time to process a range of " << resVect.size() << " elements with std::vector<int> : "
              << (static_cast<double>(vecTime) / CLOCKS_PER_SEC) * 1000000.0 << " us" << std::endl;
    std::cout << "Time to process a range of " << resDeq.size() << " elements with std::deque<int> : "
              << (static_cast<double>(deqTime) / CLOCKS_PER_SEC) * 1000000.0 << " us" << std::endl;
}

/*----------------------------------------------------------------------------------*/
/*                                EXCEPTIONS                                        */
/*----------------------------------------------------------------------------------*/

const char *PmergeMe::InvalidInput::what() const throw()
{
    return ("Error");
}
