/*jslint browser */

// CS 3312, spring 2022
// Studio 8
// YOUR NAME: Ian Sokolnicki

// All the code below will be run once the page content finishes loading.
document.addEventListener('DOMContentLoaded', function () {
   'use strict';

   (function () {

      // Create a factory that holds the number of changes made to a text box and what that text is.
      const createTextKeeper = function (oldState) {
         let state; // Used to keep track of an object a mutaable string.

         // Default starting state.
         state = {
            numChangesMade: 0,
            savedText: ''
         };

         // If there is a valid previous state, use it instead.
         if (typeof oldState === 'string') {
            try {
               state = JSON.parse(oldState);
            } catch (ignore) {
            }
         }

         // The self object contains public methods.
         const self = {
            getNumChangesMade: function () {
               return state.numChangesMade;
            },
            getSavedText: function () {
               return state.savedText;
            },
            getState: function () {
               // Return a string representation of the state object, to be used for web storage.
               return JSON.stringify(state);
            },
            saveNewText: function (newText) {
               state.savedText = newText;
               state.numChangesMade += 1;
            }
         };
         // Freeze the self object to keep it from being modified later.
         return Object.freeze(self);
      };

      (function () {
         let textKeeper;

         // Create a function that updates everything that needs updating whenever the model changes.
         const updateTextKeeper = function () {

            // Save the new state in web storage.
            localStorage.setItem('CS 3312 Studio 8 sticky text', textKeeper.getState());

            // Update the view.
            document.querySelector('#text-input').value = textKeeper.getSavedText();
            document.querySelector('#text-changes-made').textContent = textKeeper.getNumChangesMade();
         };

         // Create a click handler for the text box.
         document.querySelector('#text-input').addEventListener('input', function () {
            // Update the model with the new text.
            textKeeper.saveNewText(document.querySelector('#text-input').value);
            // Update everything else based on the new model state.
            updateTextKeeper();
         });

         // When the page is loaded, get any saved state from web storage and use it to create a new model.
         textKeeper = createTextKeeper(localStorage.getItem('CS 3312 Studio 8 sticky text'));
         // Update everything else based on the new model state.
         updateTextKeeper();
      }());
   }());

   (function () {

      // Create a factory that makes an object to make a card clicker/duplicator.
      const createCounter = function (oldState) {
         let state; // Used to keep track of an object with countable cards.

         // Create a default starting state with a count of 1.
         state = {
            count: 1
         };
         // If there's a valid previous state, use it instead.
         if (typeof oldState === 'string') {
            try {
               state = JSON.parse(oldState);
            } catch (ignore) {
            }
         }

         // The self object contains public methods.
         const self = {
            getCount: function () {
               return state.count;
            },
            getState: function () {
               // Return a string representation of the state object, to be used for web storage.
               return JSON.stringify(state);
            },
            increment: function (num) {
               // Increment count by a passed in number.
               state.count += num;
            }
         };
         // Normally it's best to freeze the self object to keep it from being modified later.
         return Object.freeze(self);
      };

      (function () {
         let cardCounter;

         // Create a function that updates everything that needs updating whenever the model changes.
         const updateCards = function () {

            // Save the new state in web storage.
            localStorage.setItem('CS 3312 Studio 8 card clicker', cardCounter.getState());

            // Update the view.
            const cardSetOutputElement = document.querySelector('#cards');
            // Empty the #cards element of all child elements.
            [...cardSetOutputElement.childNodes].forEach(function (childNode) {
               childNode.remove();
            });
            // Insert the div items as new div elements one by one.
            Array.from(
               {length: cardCounter.getCount()},
               // Create each new div element in HTML.
               function () {
                  return document.createElement('div');
               }
            ).forEach(function (divElement, whichItem) {
               // Give it its card number.
               divElement.textContent = whichItem + 1;
               // Insert it just inside the end of the array.
               cardSetOutputElement.append(divElement);
            });

            // Update the controller:  Add a click handler to each new div element.
            [...cardSetOutputElement.querySelectorAll('div')].forEach(function (divElement, whichItem) {
               divElement.addEventListener('click', function () {
                  // Update the model by increasing the number of cards by the number of the clicked card.
                  cardCounter.increment(whichItem + 1);
                  // Update everything else based on the new model state.
                  updateCards();
               });
            });
         };

         // Set up the controller:  Handle resetting the card count.
         document.querySelector('#reset-cards').addEventListener('click', function () {
            // Create a new cardCounter.
            cardCounter = createCounter();
            // Update everything else based on the new model state.
            updateCards();
         });

         // When the page is loaded, get any saved state from web storage and use it to create a new model.
         cardCounter = createCounter(localStorage.getItem('CS 3312 Studio 8 card clicker'));
         // Update everything else based on the new model state.
         updateCards();

      }());
   }());

});
