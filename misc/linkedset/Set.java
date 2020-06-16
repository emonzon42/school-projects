import java.util.Iterator;

public interface Set<E> {
   
   /**
      Removes all of the elements from this set
   */
   void clear();
   
   /**
      Returns true if this set contains no elements
      @return true if this set contains no elements
   */
   boolean isEmpty();
   
   /**
      Returns the number of elements in this set (its cardinality)
      @return the number of elements in this set
   */
   int size();
   
   /**
      Returns an iterator over the elements in this set
      @return an iterator over the elements in this set
   */
   Iterator<E> iterator();

   /**
      Returns true if this set contains the specified element
      @param e element whose presence in this set is to be tested
      @return true if this set contains the specified element
   */
   boolean contains (E e);
   
   /**
      Adds the specified element to this set if it is not already present
      @param e element to be added to this set
      @return true if this set did not already contain the specified element
   */
   boolean add (E e);
   
   /**
      Removes the specified element from this set if it is present
      @param e element to be removed from this set, if present
      @return true if this set contained the specified element
   */
   boolean remove (E e);
}
   