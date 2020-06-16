import java.util.*;

public class SOLinkedSet<E> implements Set<E> {

    private class Node<E> {

        private E element;
        private Node<E> next;

        public Node(E e) {
            this(e, null);
        }

        public Node(E e, Node next) {
            this.element = e;
            this.next = next;
        }

    }

    private class LinkedSetIterator implements Iterator<E> {

        private Node<E> current;
        private Node<E> previous;

        // LinkedSetIterator
        private LinkedSetIterator() {
            current = first;
            previous = null;
        }

        // hasNext()
        public boolean hasNext() {
            return (current != null);
        }

        // next()
        public E next() {
            previous = current;
            E old = current.element;
            current = current.next;
            return old;
        }

        public boolean hasPrevious() {
            return (previous != null);
        }

        public E previous() {
            current = previous;
            E old = previous.element;
            return old;
        }

        // remove()
        public void remove() {
            previous = null;
        }

    }

    private Node<E> first;
    private Node<E> last;

    public SOLinkedSet() {
        first = null;
        last = null;
    }

    // void clear
    public void clear() {
        first = null;
        last = null;
    }

    // boolean isEmpty
    public boolean isEmpty() {
        return (first == null);
    }

    // int size
    public int size() {
        int total = 0;
        LinkedSetIterator lit = iterator();
        while (lit.hasNext()) {
            ++total;
            lit.next();
        }
        return total;
    }

    // iterator iterator
    public LinkedSetIterator iterator() {
        return new LinkedSetIterator();
    }

    // boolean contains(E e)
    public boolean contains(E e) {
        E temp = e;
        Node tempNode;

        if (isEmpty())
            return false;
        if (e.equals(first.element)) {
            remove(e);
            tempNode = first;
            first = new Node<E>(temp, tempNode);
            //first = first.next;
            if (first == null)
                last = null;
            return true;
        }

        Node<E> pred = first;
        while (pred.next != null && !pred.next.element.equals(e)) {
            pred = pred.next;
        }
        if (pred.next == null)
            return false;
        remove(e);
        tempNode = first;
        first = new Node<E>(temp, tempNode);
        return true;
    }

    // boolean add(E e)
    public boolean add(E e){
        if (contains(e)) {
            remove(e);
            Node temp = new Node<>(e);
            temp.next = first;
            first = temp;
            return false;
        }
        if (isEmpty()) {
            first = new Node<E>(e);
            last = first;    
        } else {
            Node temp = new Node<>(e);
            temp.next = first;
            first = temp;

        }
        return true;
    }

    public boolean remove(E e) {
        if (isEmpty())
            return false;
        if (e.equals(first.element)) {
            first = first.next;
            if (first == null)
                last = null;
            return true;
        }
        Node<E> pred = first;
        while (pred.next != null && !pred.next.element.equals(e))
            pred = pred.next;
        if (pred.next == null)
            return false;
        pred.next = pred.next.next;
        if (pred.next == null)
            last = pred;
        return true;
    }

    public String toString(){
        
        StringBuilder output = new StringBuilder("[");
        Node n = first;
        while (n != null) {
            output.append(n.element);
            if (n.next != null) {
                output.append(",");
            }
            n = n.next;
        }
        return output.append("]").toString();
    }
}
